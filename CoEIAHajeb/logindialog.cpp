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
#include "logindialog.h"
#include "ui_logindialog.h"
#include "../Global/global.h"

LoginDialog::LoginDialog(QWidget *parent):
        QDialog(parent),ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setFixedSize(416,174);
}

void LoginDialog::on_loginButton_clicked() {
    if (ui->passwordLineEdit->text().isEmpty()) {
        QMessageBox::warning(this,"خطأ","من فضلك قم بادخال الباسوورد الصحيح");
        return ;
    }

    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    QString password;

    query.exec("SELECT password FROM ProgramAccountsTable;");
    if (query.next())
        password = query.value(0).toString();

    QString md5Pass = QString(QCryptographicHash::hash(ui->passwordLineEdit->text().toLatin1(),QCryptographicHash::Md5).toHex());

    if (md5Pass == password)
        this->accept();
    else {
        QMessageBox::warning(this,"خطأ","من فضلك قم بادخال الباسوورد الصحيح");
        ui->passwordLineEdit->selectAll();
    }
}


void LoginDialog::on_quitButton_clicked() {
    this->reject();
}
