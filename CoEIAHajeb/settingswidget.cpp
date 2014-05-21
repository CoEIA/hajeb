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
#include <QMessageBox>
#include <QDataWidgetMapper>
#include <QTextEdit>
#include <QPushButton>
#include <QPainter>
#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "useraccounts.h"

#include "../Global/global.h"

SettingsWidget::SettingsWidget(QWidget *parent):
        QWidget(parent),ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    settingsTitle = "اعدادات المستخدم ";
    ui->settingsGroupBox->setTitle(settingsTitle);
    statisticsTitle = "إحصائيات المستخدم ";
    ui->statGroupBox->setTitle(statisticsTitle);

    createModel();
    int index = ui->userNameComboBox->findText(getCurrentUserName());
    if (index == -1)
        index = 0 ;
    ui->userNameComboBox->setCurrentIndex(index);
    readUserSettings(index);
}

void SettingsWidget::createModel() {
    // Load model to userNameComboBox
    userNameComboBoxModel = new QSqlTableModel(this,QSqlDatabase::database(PCSDB_NAME));
    userNameComboBoxModel->setTable("UsersTable");
    userNameComboBoxModel->select();
    ui->userNameComboBox->setModel(userNameComboBoxModel);
    ui->userNameComboBox->setModelColumn(1);
    connect(ui->userNameComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(readUserSettings(int)));
}

void SettingsWidget::on_saveSettingsButton_clicked() {
    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("UPDATE SettingsTable SET programLock = ?, websiteLock = ?, browseTime = ?, monitorBrowse = ?, monitorKeyboard = ?, monitorScreen = ? WHERE userId = ?");
    query.addBindValue(ui->programLockCheckBox->isChecked()?1:0);
    query.addBindValue(ui->webLockCheckBox->isChecked()?1:0);
    query.addBindValue(ui->browseTimeCheckBox->isChecked()?1:0);
    query.addBindValue(ui->webMonitorCheckBox->isChecked()?1:0);
    query.addBindValue(ui->keylogCheckBox->isChecked()?1:0);
    query.addBindValue(ui->screenshotMonitorCheckBox->isChecked()?1:0);
    query.addBindValue(id);
    if (query.exec()) {
        QString userName = ui->userNameComboBox->currentText();
        QMessageBox::information(this,"تم الحفظ بنجاح",QString("لقد تم تحديث الإعدادات للمستخدم %1.").arg(userName));
    }
}

void SettingsWidget::readUserSettings(int index) {
    // update groupbox title
    QString title = settingsTitle + ui->userNameComboBox->currentText() + " ";
    ui->settingsGroupBox->setTitle(title);
    title = statisticsTitle + ui->userNameComboBox->currentText() + " ";
    ui->statGroupBox->setTitle(title);

    int id = ui->userNameComboBox->model()->index(index,0).data().toInt();
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT programLock,websiteLock,browseTime,monitorBrowse,monitorKeyboard,monitorScreen FROM SettingsTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next()) {
        ui->programLockCheckBox->setChecked(((query.value(0).toInt())?true:false));
        ui->webLockCheckBox->setChecked(((query.value(1).toInt())?true:false));
        ui->browseTimeCheckBox->setChecked(((query.value(2).toInt())?true:false));
        ui->webMonitorCheckBox->setChecked(((query.value(3).toInt())?true:false));
        ui->keylogCheckBox->setChecked(((query.value(4).toInt())?true:false));
        ui->screenshotMonitorCheckBox->setChecked(((query.value(5).toInt())?true:false));
        updateStatistics();
    }
}

void SettingsWidget::on_updateStatisticsButton_clicked() {
    updateStatistics();
    QString userName = ui->userNameComboBox->currentText();
    QMessageBox::information(this,"تم التحديث",QString("لقد تم تحديث الإحصائيات للمستخدم %1.").arg(userName));
}

void SettingsWidget::updateStatistics() {
    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    QString userName = ui->userNameComboBox->currentText();

    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    QSqlQuery lspQuery(QSqlDatabase::database(LSPDB_NAME));

    int blockedWebsiteNo = 0;
    int blockedProgramsNo = 0;
    int keystrokesRecordsNo = 0;

    // No of Blocked Site for current user
    lspQuery.prepare("SELECT count(*) FROM LogsTable WHERE userId = ? AND actionType = ?");
    lspQuery.addBindValue(id);
    lspQuery.addBindValue(1);
    lspQuery.exec();
    if (lspQuery.next())
        blockedWebsiteNo = lspQuery.value(0).toInt();
    ui->blockedWensiteNOLabel->setText(QString::number(blockedWebsiteNo));

    // No of Blocked Programs for current user
    query.prepare("SELECT count(*) FROM LogsTable WHERE userId = ? AND actionType = ?");
    query.addBindValue(id);
    query.addBindValue(2);
    query.exec();
    if (query.next())
        blockedProgramsNo = query.value(0).toInt();
    ui->blockedProgramsNOLabel->setText(QString::number(blockedProgramsNo));

    // No of Screenshots for current user
    QDir dir(SCREENSHOTS_PATH + "\\" + userName);
    ui->screenshotsNoLabel->setText(QString::number(dir.entryInfoList(QDir::Files).size()));

    // No of keystrokes for current user
    query.prepare("SELECT count(*) FROM KeylogsTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next())
        keystrokesRecordsNo = query.value(0).toInt();
    ui->keystrokesRecordsNOLabel->setText(QString::number(keystrokesRecordsNo));
}

void SettingsWidget::updateUsersModel() {
    int idx = ui->userNameComboBox->currentIndex();
    userNameComboBoxModel->select();
    if (idx < 0 || idx >= ui->userNameComboBox->count())
        idx = 0 ;
    ui->userNameComboBox->setCurrentIndex(idx);
}

