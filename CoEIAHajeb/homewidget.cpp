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
#include <QInputDialog>
#include <QProcess>

#include "homewidget.h"
#include "ui_homewidget.h"
#include "resettingregistry.h"
#include "hajebservices.h"
#include "useraccounts.h"
#include "server.h"
#include "constant.h"
#include "applicationstatus.h"
#include "useraccountsdialog.h"
#include "updatehandler.h"
#include "luncher.h"

#include "../Global/global.h"
#include "../Global/keyvalidator.h"


HomeWidget::HomeWidget(QWidget *parent):
    QWidget(parent),ui(new Ui::HomeWidget),userAccountsWidget(0)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    readTaskMangerStatus();
    readHajebServicesStatus();
    updateHajebVersionLabels();
    readApplicationStatus();

    connect(ui->taskManagerCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableTaskManger(bool)));
    connect(ui->hajebservicesButton,SIGNAL(toggled(bool)),this,SLOT(hajebservicesButtonToggled(bool)));
    connect(this,SIGNAL(registerd()),this,SLOT(readApplicationStatus()));

    QTimer::singleShot(0,this,SLOT(checkForUpdate()));
}

void HomeWidget::readTaskMangerStatus() {
    // Read TaskManger status, if enabled set taskManagerCheckBox to true.
    if (isTaskManagerEnable())
        ui->taskManagerCheckBox->setChecked(true);
    else
        ui->taskManagerCheckBox->setChecked(false);
}

void HomeWidget::readHajebServicesStatus() {
    // Check if Hajeb Service is running , then set the button to checked
    bool serviceIsEnabled = HajebServices::isServiceRunning();

    if (serviceIsEnabled) {
        ui->hajebservicesButton->setChecked(true);
        ui->hajebservicesButton->setText("تعطيل خدمات حاجب");
        ui->hajebservicesButton->setIcon(QIcon(":/images/disable.png"));
    } else {
        ui->hajebservicesButton->setChecked(false);
        ui->hajebservicesButton->setText("تفعيل خدمات حاجب");
        ui->hajebservicesButton->setIcon(QIcon(":/images/ok.png"));
    }
}

void HomeWidget::enableTaskManger(bool isChecked) {
    // Check if current login user has admin right ?
    QString currentUserName = getCurrentUserName();
    QStringList adminNames = getAllAdminsName();
    if (!adminNames.contains(currentUserName)) {
        QMessageBox::critical(this,"لا يمكن تنفيذ العملية من حساب هذا المستخدم","لا يمكن تنفيذ العملية الا من خلال تسجيل الدخول لحساب مدير النظام");
        return ;
    }

    if (isChecked) {
        // Enable Task Manager
        enableTaskManager();
        QMessageBox::information(this,"تم تفعيل مدير المهام","تم تفعيل مدير المهام بنجاح");
    } else {
        // Disable Task Manager
        if (!disableTaskManager()) {
            createHKLMDisabledTaskManagerKey();
            createHKCUDisabledTaskManagerKey();
        }
        QMessageBox::information(this,"تم تعطيل مدير المهام","تم تعطيل مدير المهام بنجاح");
    }
}

void HomeWidget::hajebservicesButtonToggled(bool isChecked) {
    // Check if current login user has admin right ?
    QString currentUserName = getCurrentUserName();
    QStringList adminNames = getAllAdminsName();
    if (!adminNames.contains(currentUserName)) {
        QMessageBox::critical(this,"لا يمكن تنفيذ العملية من حساب هذا المستخدم","لا يمكن تنفيذ العملية الا من خلال تسجيل الدخول لحساب مدير النظام");
        return ;
    }

    if (isChecked) {
        // Enable Services
        ui->hajebservicesButton->setText("تعطيل خدمات حاجب");
        ui->hajebservicesButton->setIcon(QIcon(":/images/disable.png"));

        if (QSysInfo::windowsVersion() == QSysInfo::WV_XP || QSysInfo::windowsVersion() == QSysInfo::WV_2003)
            HajebServices::startHajebServicesWindows5();
        else {
            //HajebServices::installHajebServicesWindows6();
            HajebServices::startHajebServicesWindows6();
        }

        QMessageBox::information(this,"تم تفعيل الخدمات","تم تشغيل خدمات حاجب بنجاح");

    } else {
        // Disable Services
        ui->hajebservicesButton->setText("تفعيل خدمات حاجب");
        ui->hajebservicesButton->setIcon(QIcon(":/images/ok.png"));

        if (QSysInfo::windowsVersion() == QSysInfo::WV_XP || QSysInfo::windowsVersion() == QSysInfo::WV_2003)
            HajebServices::stopHajebServicesWindows5();
        else {
            HajebServices::stopHajebServicesWindows6();
            //HajebServices::uninstallHajebServicesWindows6();
        }

        QMessageBox::information(this,"تم تعطيل الخدمات","تم تعطيل خدمات حاجب بنجاح");
    }
}

void HomeWidget::on_userButton_clicked() {
    // Check if current login user has admin right ?
    QString currentUserName = getCurrentUserName();
    QStringList adminNames = getAllAdminsName();
    if (!adminNames.contains(currentUserName)) {
        QMessageBox::critical(this,"لا يمكن تنفيذ العملية من حساب هذا المستخدم","لا يمكن تنفيذ العملية الا من خلال تسجيل الدخول لحساب مدير النظام");
        return ;
    }

    userAccountsWidget = new UserAccountsWidget(this);

    // Read Previous Choosed User.
    userAccountsWidget->setCurrentUsers(userAccountsWidget->currentUsers());

    if (userAccountsWidget->exec() == QDialog::Accepted)
        emit updateModel();

    delete userAccountsWidget;
}

void HomeWidget::on_registerKeyButton_clicked() {
    // Check if current login user has admin right ?
    QString currentUserName = getCurrentUserName();
    QStringList adminNames = getAllAdminsName();
    if (!adminNames.contains(currentUserName)) {
        QMessageBox::critical(this,"لا يمكن تنفيذ العملية من حساب هذا المستخدم","لا يمكن تنفيذ العملية الا من خلال تسجيل الدخول لحساب مدير النظام");
        return ;
    }

    bool ok;
    QString serialNumber = QInputDialog::getText(this,"تسجيل البرنامج","أدخل رقم تسجيل البرنامج",QLineEdit::Normal,"",&ok);
    if (!ok || serialNumber.isEmpty())
        return ;
    verifyKey(serialNumber);
}

void HomeWidget::verifyKey(QString serialNumber) {
    if (KeyValidator::validate(serialNumber)) {
        KeyType state = Server::verifyKey(serialNumber);
        if (state == CLEAN_KEY) {
            if (Server::registerKeyOnline(serialNumber)) {
                QMessageBox::information(this,"تم التسجيل بنجاح","لقد تم تسجيل نسخة حاجب");
                ApplicationStatus::registerApplication(serialNumber,true);
                emit registerd();
            } else
                QMessageBox::warning(this,"لم يتم تسجيل نسخة حاجب","لقد فشلت عملية تسجيل حاجب نرجوا المحاولة مجددا.");

        } else if (state == USED_KEY || state == ILLEGAL_KEY) {
            QMessageBox::warning(this,"لا يمكن تسجيل نسخة حاجب","عفوا ، رقم التسجيل غير صحيح.\nتأكد من ادخال الرقم بشكل صحيح.");

        } else if (state == UNKNOWN_KEY) {
            QMessageBox::information(this,"لقد تم تسجيل نسخة حاجب","تم تسجيل النسخة بشكل مؤقت الى ان يتم التأكد مرة اخرى عند الاتصال بالانترنت");
            ApplicationStatus::registerApplication(serialNumber,false);
        }

    } else
        QMessageBox::warning(this,"لا يمكن تسجيل نسخة حاجب","عفوا ، رقم التسجيل غير صحيح.\nتأكد من ادخال الرقم بشكل صحيح.");
}

void HomeWidget::on_updateButton_clicked() {
    // Check if current login user has admin right ?
    QString currentUserName = getCurrentUserName();
    QStringList adminNames = getAllAdminsName();
    if (!adminNames.contains(currentUserName)) {
        QMessageBox::critical(this,"لا يمكن تنفيذ العملية من حساب هذا المستخدم","لا يمكن تنفيذ العملية الا من خلال تسجيل الدخول لحساب مدير النظام");
        return ;
    }

    UpdateHandler updateHandler;
    UpdateHandler::Response response = updateHandler.checkForUpdate();
    if (response == UpdateHandler::CONNECTION_ERROR) {
        QMessageBox::warning(this,"خطأ في الإتصال","لا يوجد اتصال مع الخادم ، حاول لاحقا.");
    } else if (response == UpdateHandler::NO_UPDATE) {
        QMessageBox::information(this,"النسخة محدثة بالكامل","لا توجد تحديثات جديدة.\nسوف يتم تنبيهك عند توفر تحديث جديد لحاجب");
    } else {
        int ret = QMessageBox::information(this,"يوجد تحديث لحاجب","توجد ملفات تحديث لبرنامج حاجب.\nننصحك ببدء عملية التحديث فورا.",QMessageBox::Yes|QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            QStringList files = updateHandler.newFilesToUpdate();
            qDebug() << files;
            HajebServices::uninstall();
            Luncher::lunchUpdate(files);
            qApp->quit();
        }
    }
}

void HomeWidget::updateHajebVersionLabels() {
    QFile file("version.txt");
    if (!file.open(QIODevice::ReadOnly))
        return ;
    QStringList list = QString(file.readAll()).split("\n");
    ui->coeiaHajebVerLabel->setText("version: "+list[0].split(" ")[1].trimmed());
    ui->coeiaLSPVerLabel->setText("version: "+list[1].split(" ")[1].trimmed());
    ui->coeiaKeyloggerVerLabel->setText("version: "+list[2].split(" ")[1].trimmed());
    ui->coeiaProcessMonitorVerLabel->setText("version: "+list[3].split(" ")[1].trimmed());
    ui->coeiaScreenshotMonitorVerLabel->setText("version: "+list[4].split(" ")[1].trimmed());
    ui->coeiaNetworkMonitorVerLabel->setText("version: "+list[5].split(" ")[1].trimmed());
    ui->coeiaHajebRemoverVerLabel->setText("version: "+list[6].split(" ")[1].trimmed());
}

void HomeWidget::readApplicationStatus() {
    if (ApplicationStatus::isCleanVersion()) {
        ui->registerKeyButton->setEnabled(false);
        ui->registerKeyButton->setText("تم تسجيل البرنامج ");
    }
}


void HomeWidget::checkForUpdate() {
    // Check if current login user has admin right ?
    QString currentUserName = getCurrentUserName();
    QStringList adminNames = getAllAdminsName();
    if (!adminNames.contains(currentUserName)) {
        QMessageBox::critical(this,"لا يمكن تنفيذ العملية من حساب هذا المستخدم","لا يمكن تنفيذ العملية الا من خلال تسجيل الدخول لحساب مدير النظام");
        return ;
    }

    UpdateHandler updateHandler;
    UpdateHandler::Response response = updateHandler.checkForUpdate();
    if (response == UpdateHandler::UPDATE) {
        int ret = QMessageBox::information(this,"يوجد تحديث لحاجب","توجد ملفات تحديث لبرنامج حاجب.\nننصحك ببدء عملية التحديث فورا.",QMessageBox::Ok|QMessageBox::Cancel);
        if (ret == QMessageBox::Yes) {
            QStringList files = updateHandler.newFilesToUpdate();
            qDebug() << files;
            HajebServices::uninstall();
            Luncher::lunchUpdate(files);
            qApp->quit();
        }
    }
}

