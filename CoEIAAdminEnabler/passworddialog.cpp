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

#include <QMessageBox>
#include "passworddialog.h"
#include "ui_passworddialog.h"

PasswordDialog::PasswordDialog(QWidget *parent):
        QDialog(parent),ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
}

QString PasswordDialog::pwd1() const {
    return ui->password1LineEdit->text();
}

QString PasswordDialog::pwd2() const {
    return ui->password2LineEdit->text();
}


void PasswordDialog::on_saveButton_clicked() {
    if (pwd1() != pwd2()) {
        QMessageBox::warning(this,"كلمة المرور غير صحيحة","ادخل كلمة المرور بشكل صحيح");
        ui->password1LineEdit->clear();
        ui->password2LineEdit->clear();
        return ;
    }

    if (pwd1().isEmpty() || pwd2().isEmpty()) {
        QMessageBox::warning(this,"ادخل كلمة المرور","لا يمكن وضع كلمة مرور فارغة");
        return ;
    }
    this->accept();
}
