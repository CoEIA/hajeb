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

#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include "adminenablerwidget.h"
#include "ui_adminenablerwidget.h"
#include "passworddialog.h"

AdminEnablerWidget::AdminEnablerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminEnablerWidget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/Administrator128.png"));
}

AdminEnablerWidget::~AdminEnablerWidget()
{
    delete ui;
}

void AdminEnablerWidget::on_enableAdminButton_clicked() {
    PasswordDialog* dlg = new PasswordDialog(this);
    if (dlg->exec() != QDialog::Accepted)
        return ;
    QString pwd1 = dlg->pwd1();
    QString pwd2 = dlg->pwd2();

    // Set Admin Password
    QProcess* process = new QProcess(this);
    QString adminPass = "net user administrator " + pwd1;
    process->start(adminPass);
    process->waitForStarted();
    if (!process->waitForFinished()) {
        QMessageBox::information(this,"تفعيل حساب المدير","لم تنجح عملية تفعيل حساب مدير النظام");
        return ;
    }

    QString output = process->readAll();
    if (output.contains("Access is denied.")) {
        QMessageBox::information(this,"تفعيل حساب المدير","لم تنجح عملية تفعيل حساب مدير النظام");
        return ;
    }

    // Enable Admin Account.

    QString enableAdmin = "net user administrator /active:yes";
    process->start(enableAdmin);
    process->waitForStarted();
    if (!process->waitForFinished()) {
        QMessageBox::information(this,"تفعيل حساب المدير","لم تنجح عملية تفعيل حساب مدير النظام");
        return ;
    }

    output = process->readAll();
    if (output.contains("Access is denied."))
        QMessageBox::information(this,"تفعيل حساب المدير","لم تنجح عملية تفعيل حساب مدير النظام");
    else
        QMessageBox::information(this,"تفعيل حساب المدير","تم تفعيل حساب مدير النظام بنجاح");
}

void AdminEnablerWidget::on_disableAdminButton_clicked() {
    QProcess* process = new QProcess(this);
    QString disableAdmin = "net user administrator /active:no";
    process->start(disableAdmin);
    process->waitForStarted();
    if (!process->waitForFinished()) {
        QMessageBox::information(this,"تعطيل حساب المدير","لم تنجح عملية تعطيل حساب مدير النظام");
        return ;
    }

    QString output = process->readAll();
    if (output.contains("Access is denied."))
        QMessageBox::information(this,"تعطيل حساب المدير","لم تنجح عملية تعطيل حساب مدير النظام");
    else
        QMessageBox::information(this,"تعطيل حساب المدير","تم تعطيل حساب مدير النظام بنجاح");
}

void AdminEnablerWidget::on_exitButton_clicked() {
    qApp->quit();
}
