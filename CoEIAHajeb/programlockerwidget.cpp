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
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QProgressDialog>
#include <QDebug>
#include "programlockerwidget.h"
#include "ui_programlockerwidget.h"
#include "addprogramdialog.h"
#include "useraccounts.h"
#include "../Global/global.h"

ProgramLockerWidget::ProgramLockerWidget(QWidget *parent):
        QWidget(parent),ui(new Ui::ProgramLockerWidget)
{
    ui->setupUi(this);
    programTitle = "قائمة البرامج المحظورة للمستخدم ";
    ui->programGroupBox->setTitle(programTitle);

    addProgramDialog = 0;

    createModel();
    initView();
    int index = ui->userNameComboBox->findText(getCurrentUserName());
    if (index == -1)
        index = 0 ;
    ui->userNameComboBox->setCurrentIndex(index);
    readUserData(index);
}

void ProgramLockerWidget::createModel() {
    model = new QSqlTableModel(this,QSqlDatabase::database(PCSDB_NAME));
    model->setTable("LockedProgramsTable");
    model->setHeaderData(2,Qt::Horizontal,"اسم العملية");
    model->setHeaderData(3,Qt::Horizontal,"اسم البرنامج");
    model->setHeaderData(4,Qt::Horizontal,"تاريخ الحجب");
    model->select();

    // Load model to userNameComboBox
    userNameComboBoxModel = new QSqlTableModel(this,QSqlDatabase::database(PCSDB_NAME));
    userNameComboBoxModel->setTable("UsersTable");
    userNameComboBoxModel->select();
    ui->userNameComboBox->setModel(userNameComboBoxModel);
    ui->userNameComboBox->setModelColumn(1);
    connect(ui->userNameComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(readUserData(int)));
}

void ProgramLockerWidget::initView() {
    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    QHeaderView* headerView = ui->tableView->horizontalHeader();
    headerView->setResizeMode(2,QHeaderView::ResizeToContents);
    headerView->setStretchLastSection(true);
}

void ProgramLockerWidget::on_addButton_clicked() {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    bool buildList = false;
    if (!addProgramDialog) {
        addProgramDialog = new AddProgramDialog(this);
        connect(addProgramDialog,SIGNAL(programListChoosed()),SLOT(saveProgramList()));
        connect(addProgramDialog,SIGNAL(processChoosed()),SLOT(saveProcess()));
        buildList = true;
    }

    addProgramDialog->show();

    if (buildList)
        addProgramDialog->build();
    QApplication::restoreOverrideCursor();
}

void ProgramLockerWidget::save(QString processName) {
    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    QString name = ui->userNameComboBox->currentText();

    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("INSERT INTO LockedProgramsTable(userId,processName,programName,lockDate) VALUES (?,?,?,?);");
    query.addBindValue(id);
    query.addBindValue(processName);
    query.addBindValue(processName.remove(".exe"));
    query.addBindValue(QDate::currentDate());
    query.exec();

    model->select();

//    QMessageBox::information(this,"تم الحفظ بنجاح",QString("تم إضافة البرنامج %1 الى قائمة البرامج المحظورة للمستخدم %2").arg(processName.remove(".exe")).arg(name));
}

void ProgramLockerWidget::on_deleteButton_clicked() {
    if (ui->tableView->currentIndex() == QModelIndex()) {
        QMessageBox::warning(this,"لا يمكن الحذف","قم بتحديد برنامج من القائمة لكي يتم حذفه");
        return ;
    }
    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    int no = model->index(ui->tableView->currentIndex().row(),0).data().toInt();

    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("DELETE FROM LockedProgramsTable WHERE userId = ? AND no = ?");
    query.addBindValue(id);
    query.addBindValue(no);
    query.exec();

    model->setFilter(QString("LockedProgramsTable.userId = %1").arg(id));
}

void ProgramLockerWidget::readUserData(int index) {
    // update groupbox title
    QString title = programTitle + ui->userNameComboBox->currentText() + " ";
    ui->programGroupBox->setTitle(title);

    int id = ui->userNameComboBox->model()->index(index,0).data().toInt();
    model->setFilter(QString("LockedProgramsTable.userId = %1").arg(id));
}

void ProgramLockerWidget::updateUsersModel() {
    int idx = ui->userNameComboBox->currentIndex();
    userNameComboBoxModel->select();
    if (idx < 0 || idx >= ui->userNameComboBox->count())
        idx = 0 ;
    ui->userNameComboBox->setCurrentIndex(idx);
}

void ProgramLockerWidget::saveProcess() {
    QString processName = addProgramDialog->processName();

    if (processName.isEmpty()) {
        QMessageBox::warning(this,"لا يمكن اضافة السجل","قم بتحديد البرنامج المراد حظره");
        return ;
    }

    save(processName);
}

void ProgramLockerWidget::saveProgramList() {
    QStringList programList = addProgramDialog->programList();

    foreach (QString program, programList)
        save(program);
}

