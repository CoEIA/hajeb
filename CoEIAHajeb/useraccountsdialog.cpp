/*
* Copyright (C) 2014 - Center of Excellence in Information Assurance
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QtSql>
#include <QtGui>

#include "hajebservices.h"
#include "useraccounts.h"
#include "useraccountsdialog.h"
#include "wizarddialog.h"
#include "../Global/global.h"

UserAccountsWidget::UserAccountsWidget(QWidget* parent):
        QDialog(parent)
{
    userPageWidget = new UserPageWidget;
    QPushButton* saveButton = new QPushButton("حفظ");
    saveButton->setIcon(QIcon(":/images/save.png"));

    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveButtonClicked()));

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(saveButton);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(userPageWidget);
    layout->addLayout(bottomLayout);
}

void UserAccountsWidget::saveButtonClicked() {
    QStringList users = userPageWidget->userAccounts();
    QStringList dbUsers = currentUsers();

    QStringList usersToInsert;
    QStringList usersToDelete;

    // New users
    foreach (QString user,users) {
        if (!dbUsers.contains(user))
            usersToInsert << user;
    }

    // Removed Users
    foreach (QString dbUser,dbUsers) {
        if (!users.contains(dbUser))
            usersToDelete << dbUser;
    }

    if (!usersToInsert.size() && !usersToDelete.size()) {
        QMessageBox::information(this,"لا يوجد تغييرات","بيانات المستخدمين محدثة");
        return ;
    }

    this->hide();
    LogDialog* logDialog = new LogDialog(this);
    connect(this,SIGNAL(logResult(QString)),logDialog,SLOT(log(QString)));
    logDialog->show();


    HajebServices::uninstall();

    // Add User
    if (usersToInsert.size())
        insertUsers(usersToInsert);

    // Remove User
    if (usersToDelete.size())
        deleteUsers(usersToDelete);


    HajebServices::install();

    logDialog->hide();
    delete logDialog;
    this->accept();
}

void UserAccountsWidget::setCurrentUsers(QStringList currentUsers) {
    userPageWidget->setCurrentUsers(currentUsers);
}

QStringList UserAccountsWidget::currentUsers() {
    QStringList currentUsers;
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.exec("SELECT name FROM UsersTable");
    while (query.next())
        currentUsers << query.value(0).toString();
    return currentUsers;
}

void UserAccountsWidget::insertUsers(QStringList usersToInsert) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    QSqlQuery LSPquery(QSqlDatabase::database(LSPDB_NAME));

    foreach (QString user, usersToInsert) {
        query.prepare("INSERT INTO UsersTable(name) VALUES (?) ;");
        query.addBindValue(user);
        if (query.exec())
            emit logResult(QString("إنشاء ملف للمستخدم: %1 ...").arg(user));
    }

    foreach (QString user, usersToInsert) {
        LSPquery.prepare("INSERT INTO UsersTable(name) VALUES (?) ;");
        LSPquery.addBindValue(user);
        LSPquery.exec();
    }

    qApp->processEvents();

    foreach (QString user, usersToInsert) {
        query.prepare("SELECT userId FROM UsersTable WHERE name = ?");
        query.addBindValue(user);
        query.exec();
        query.next();
        int id = query.value(0).toInt();
        query.prepare("INSERT INTO SettingsTable(userId,programLock,websiteLock,browseTime,monitorBrowse,monitorKeyboard,monitorScreen,browseTimeMethod) VALUES (?,?,?,?,?,?,?,?) ;");
        query.addBindValue(id);
        query.addBindValue(1);
        query.addBindValue(1);
        query.addBindValue(1);
        query.addBindValue(1);
        query.addBindValue(1);
        query.addBindValue(1);
        query.addBindValue(0);  /* 0 = TimeTable, 1 = Counter */
        if (query.exec())
            emit logResult(QString("تفعيل إعدادات المستخدم: %1 ...").arg(user));
        qApp->processEvents();
        for (int day=1; day < 8; day++) {
            query.prepare("INSERT INTO BrowseTimesTable(userId,dayNo,timetable) VALUES (?,?,16777215) ;");
            query.addBindValue(id);
            query.addBindValue(day);
            query.exec();
        }

        for (int day=1; day < 8; day++) {
            query.prepare("INSERT INTO BrowseTimesCounterTable(userId,dayNo,allowedTime,remainingTime,lastUseDate) VALUES (?,?,?,?,?) ;");
            query.addBindValue(id);
            query.addBindValue(day);
            query.addBindValue(QTime(23,59,59));
            query.addBindValue(QTime(23,59,59));
            query.addBindValue(QDate::currentDate());
            query.exec();
        }

        query.prepare("INSERT INTO ScreenshotsTable(userId,timePeriod) VALUES (?,5) ;");
        query.addBindValue(id);
        query.exec();

        // Block chrome.exe for all non-admin user.
        QStringList adminNames = getAllAdminsName();
        if (!adminNames.contains(user)) {
            query.prepare("INSERT INTO LockedProgramsTable(userId,processName,programName,lockDate) VALUES (?,?,?,?) ;");
            query.addBindValue(id);
            query.addBindValue("chrome.exe");
            query.addBindValue("chrome");
            query.addBindValue(QDate::currentDate());
            query.exec();
        }
    }

    qApp->processEvents();
}

void UserAccountsWidget::deleteUsers(QStringList userstoDelete) {
    qApp->processEvents();
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    QSqlQuery LSPquery(QSqlDatabase::database(LSPDB_NAME));
    query.exec("PRAGMA foreign_keys = ON;");    // Inforece foreing key constrain.

    qApp->processEvents();

    foreach (QString user, userstoDelete) {
        query.prepare("SELECT userId FROM UsersTable WHERE name = ?");
        query.addBindValue(user);
        query.exec();
        query.next();
        int id = query.value(0).toInt();

        // Remove from Hajeb DB
        query.prepare("DELETE FROM UsersTable WHERE userId = ?");
        query.addBindValue(id);
        if (query.exec()) {
            emit logResult(QString("<b>حذف ملف المستخدم: %1 ...</b>").arg(user));
        } else
            qDebug() << query.lastError();

        qApp->processEvents();

        // Remove from LSP DB
        LSPquery.prepare("DELETE FROM UsersTable WHERE userId = ?");
        LSPquery.addBindValue(id);
        LSPquery.exec();

        qApp->processEvents();

        // Remove Screenshots
        QString path = INSTALL_PATH + "\\Screenshots\\" + user;
        QDir dir(path);
        foreach (QFileInfo fileInfo,dir.entryInfoList(QDir::Files)) {
            qDebug() << "remove " << fileInfo.fileName() << " : " << dir.remove(fileInfo.fileName());
        }

        qApp->processEvents();
    }
}

LogDialog::LogDialog(QWidget *parent):
        QDialog(parent)
{
    textEdit = new QTextEdit;
    okButton = new QPushButton("انهاء");
    okButton->setEnabled(false);

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(okButton);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);
    layout->addLayout(bottomLayout);
}

void LogDialog::log(QString string) {
    textEdit->append(string);
}



