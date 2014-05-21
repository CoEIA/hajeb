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
