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
