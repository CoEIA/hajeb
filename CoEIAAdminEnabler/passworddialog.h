#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
    class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT
public:
    PasswordDialog(QWidget* parent = 0);
    QString pwd1() const ;
    QString pwd2() const ;
private slots:
    void on_saveButton_clicked();
private:
    Ui::PasswordDialog* ui;
};

#endif // PASSWORDDIALOG_H
