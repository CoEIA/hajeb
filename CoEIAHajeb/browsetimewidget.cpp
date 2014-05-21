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
#include <QPair>
#include <QDebug>
#include "browsetimewidget.h"
#include "ui_browsetimewidget.h"
#include "useraccounts.h"
#include "../Global/global.h"

BrowseTimeWidget::BrowseTimeWidget(QWidget *parent):
        QWidget(parent),ui(new Ui::BrowseTimeWidget)
{
    ui->setupUi(this);
    createModel();

    connect(ui->timesTableRadioButton,SIGNAL(clicked()),this,SLOT(setPage()));
    connect(ui->timesCounterRadioButton,SIGNAL(clicked()),this,SLOT(setPage()));

    groupBoxTitle = "قم بتحديد الساعات التي ترغب بتعطيل الانترنت فيها للمستخدم ";
    counterGroupBoxTitle = "عداد الإنترنت للمستخدم ";

    ui->timesGroupBox->setTitle(groupBoxTitle);
    ui->timesCounterGroupBox->setTitle(counterGroupBoxTitle);

    int index = ui->userNameComboBox->findText(getCurrentUserName());
    if (index == -1)
        index = 0 ;
    ui->userNameComboBox->setCurrentIndex(index);

    readSettings(index);

    // Minimize timeTable button size
    for (int i=1; i<8; ++i) {
        for (int j=0; j<24; ++j) {
            QString buttonName = "button_" + QString::number(i) + "_" + QString::number(j);
            ui->timesGroupBox->findChild<QToolButton*>(buttonName)->setIconSize(QSize(8,8));
        }
    }
}

BrowseTimeWidget::BrowseTimeMethod BrowseTimeWidget::browseTimeMethod(int index) {
    int id = ui->userNameComboBox->model()->index(index,0).data().toInt();
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT browseTimeMethod FROM SettingsTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    return (BrowseTimeWidget::BrowseTimeMethod) query.value(0).toInt();
}

void BrowseTimeWidget::setBrowseTimeMethod(int id,BrowseTimeWidget::BrowseTimeMethod method) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("UPDATE SettingsTable SET browseTimeMethod = ? WHERE userId = ?");
    query.addBindValue((int)method);
    query.addBindValue(id);
    query.exec();
}

void BrowseTimeWidget::createModel() {
    userNameComboBoxModel = new QSqlTableModel(this,QSqlDatabase::database(PCSDB_NAME));
    userNameComboBoxModel->setTable("UsersTable");
    userNameComboBoxModel->select();
    ui->userNameComboBox->setModel(userNameComboBoxModel);
    ui->userNameComboBox->setModelColumn(1);
    connect(ui->userNameComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(readSettings(int)));
}

void BrowseTimeWidget::on_saveButton_clicked() {
    int id = ui->userNameComboBox->model()->index(ui->userNameComboBox->currentIndex(),0).data().toInt();
    if (ui->timesTableRadioButton->isChecked()) {
        setBrowseTimeMethod(id,TimeTable);
        saveTimeTable(id);
    } else {
        setBrowseTimeMethod(id,TimeCounter);
        saveTimeCounter(id);
    }
}

void BrowseTimeWidget::saveTimeTable(int id) {
    QList<QPair<int,int> > timesTable;
    for (int dayNo=1; dayNo<8; ++dayNo) {
        int dayTimeTable = 0x0;
        for (int hour=0; hour<24; ++hour) {
            QString buttonName = "button_" + QString::number(dayNo) + "_" + QString::number(hour);
            if (ui->timesGroupBox->findChild<QToolButton*>(buttonName)->isChecked()) {
                int bitPos = 0x00000001;
                bitPos = bitPos << hour;
                dayTimeTable |= bitPos;
            }
        }
        QPair<int,int> pair;
        pair.first = dayNo;
        pair.second = dayTimeTable;
        timesTable << pair;
    }
    updateTimeTable(timesTable,id);
}

void BrowseTimeWidget::updateTimeTable(QList<QPair<int,int> > timesTable,int id) {
    QPair<int,int> pair;
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    foreach (pair,timesTable) {
        query.prepare("UPDATE BrowseTimesTable SET timetable = ? WHERE userId = ? and dayNo = ?");
        query.addBindValue(pair.second);
        query.addBindValue(id);
        query.addBindValue(pair.first);
        query.exec();
    }

    QString userName = ui->userNameComboBox->currentText();
    QMessageBox::information(this,"تم الحفظ بنجاح",QString("لقد تم تحديث مواعيد استخدام الانترنت للمستخدم %1.").arg(userName));
}

void BrowseTimeWidget::saveTimeCounter(int id) {
    QList<QPair<int,QTime> > timeCounters;
    for (int dayNo=1; dayNo<8; ++dayNo) {
        QString hourSpinBox = "day_h_" + QString::number(dayNo);
        QString minSpinBox = "day_m_" + QString::number(dayNo);
        int hour = ui->timesCounterGroupBox->findChild<QSpinBox*>(hourSpinBox)->value();
        int min = ui->timesCounterGroupBox->findChild<QSpinBox*>(minSpinBox)->value();
        QTime dayTimeCounter(hour,min);
        QPair<int,QTime> pair;
        pair.first = dayNo;
        pair.second = dayTimeCounter;
        timeCounters << pair;
    }
    updateTimeCounter(timeCounters,id);
}

void BrowseTimeWidget::updateTimeCounter(QList<QPair<int,QTime> > timesCounter,int id) {
    QPair<int,QTime> pair;
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    foreach (pair,timesCounter) {
        query.prepare("UPDATE BrowseTimesCounterTable SET allowedTime = ? , remainingTime = ? WHERE userId = ? and dayNo = ?");
        query.addBindValue(pair.second);
        query.addBindValue(pair.second);
        query.addBindValue(id);
        query.addBindValue(pair.first);
        query.exec();
    }

    QString userName = ui->userNameComboBox->currentText();
    QMessageBox::information(this,"تم الحفظ بنجاح",QString("لقد تم تحديث عداد استخدام الانترنت للمستخدم %1.").arg(userName));
}

void BrowseTimeWidget::on_disableAllButton_clicked() {
    if (ui->timesTableRadioButton->isChecked()) {
        QList<QToolButton*> buttonList = ui->timesGroupBox->findChildren<QToolButton*>();
        foreach (QToolButton* button,buttonList) {
            button->setChecked(false);
        }
    } else {
        QList<QSpinBox*> buttonList = ui->timesCounterGroupBox->findChildren<QSpinBox*>();
        foreach (QSpinBox* button,buttonList) {
            button->setValue(0);
        }
    }
}

void BrowseTimeWidget::on_enableAllButton_clicked() {
    if (ui->timesTableRadioButton->isChecked()) {
        QList<QToolButton*> buttonList = ui->timesGroupBox->findChildren<QToolButton*>();
        foreach (QToolButton* button,buttonList) {
            button->setChecked(true);
        }
    } else {
        QList<QSpinBox*> buttonList = ui->timesCounterGroupBox->findChildren<QSpinBox*>();
        foreach (QSpinBox* button,buttonList) {
            if (button->objectName().contains("day_h"))
                button->setValue(23);
            else
                button->setValue(59);
        }
    }
}

void BrowseTimeWidget::readSettings(int index) {
    // update groupbox title
    QString title = groupBoxTitle + ui->userNameComboBox->currentText() + " ";
    ui->timesGroupBox->setTitle(title);
    title = counterGroupBoxTitle + ui->userNameComboBox->currentText() + " ";
    ui->timesCounterGroupBox->setTitle(title);

    if (browseTimeMethod(index) == TimeTable) {
        ui->timesTableRadioButton->setChecked(true);
        ui->stackedWidget->setCurrentIndex(0);
        hideFlagButton(false);
        readUserTimeTableSettings(index);
    } else {
        ui->timesCounterRadioButton->setChecked(true);
        ui->stackedWidget->setCurrentIndex(1);
        hideFlagButton(true);
        readUserTimeCounterSettings(index);
    }
}

void BrowseTimeWidget::readUserTimeTableSettings(int index) {
    int id = ui->userNameComboBox->model()->index(index,0).data().toInt();
    QList<QPair<int,int> > timesTable;
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT dayNo,timetable FROM BrowseTimesTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    while (query.next()) {
        QPair<int,int> pair;
        pair.first = query.value(0).toInt();
        pair.second = query.value(1).toInt();
        timesTable << pair;
    }

    QPair<int,int> p;
    foreach (p, timesTable) {
        int dayNo = p.first;
        int timeTable = p.second;
        for (int i=0;i<24;++i) {
            QString buttonName = "button_" + QString::number(dayNo) + "_" + QString::number(i);
            int bitPos = 0x00000001;
            bitPos <<= i;
            ui->timesGroupBox->findChild<QToolButton*>(buttonName)->setChecked(timeTable & bitPos);
        }
    }
}

void BrowseTimeWidget::readUserTimeCounterSettings(int index) {
    int id = ui->userNameComboBox->model()->index(index,0).data().toInt();
    QList<QPair<int,QTime> > timeCounters;
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT dayNo,allowedTime FROM BrowseTimesCounterTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    while (query.next()) {
        QPair<int,QTime> pair;
        pair.first = query.value(0).toInt();
        pair.second = query.value(1).toTime();
        timeCounters << pair;
    }

    QPair<int,QTime> p;
    foreach (p, timeCounters) {
        int dayNo = p.first;
        QTime timeCounter = p.second;
        QString hourSpinBox = "day_h_" + QString::number(dayNo);
        QString minSpinBox = "day_m_" + QString::number(dayNo);
        ui->timesCounterGroupBox->findChild<QSpinBox*>(hourSpinBox)->setValue(timeCounter.hour());
        ui->timesCounterGroupBox->findChild<QSpinBox*>(minSpinBox)->setValue(timeCounter.minute());
    }
}

void BrowseTimeWidget::updateUsersModel() {
    disconnect(ui->userNameComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(readSettings(int)));
    int idx = ui->userNameComboBox->currentIndex();
    userNameComboBoxModel->select();
    if (idx < 0 || idx >= ui->userNameComboBox->count())
        idx = 0 ;
    connect(ui->userNameComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(readSettings(int)));
    ui->userNameComboBox->setCurrentIndex(idx);
}

void BrowseTimeWidget::setPage() {
    int idx = 0 ;
    if (sender() == ui->timesCounterRadioButton) {
        idx = 1;
        hideFlagButton(true);
    } else {
        hideFlagButton(false);
    }

    ui->stackedWidget->setCurrentIndex(idx);
}

void BrowseTimeWidget::hideFlagButton(bool ok) {
    ui->enableHourLabel->setHidden(ok);
    ui->disableHourLabel->setHidden(ok);
    ui->enableToolButton->setHidden(ok);
    ui->disableToolButton->setHidden(ok);
}
