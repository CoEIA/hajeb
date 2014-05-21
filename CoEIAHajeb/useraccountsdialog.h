#ifndef USERACCOUNTSDIALOG_H
#define USERACCOUNTSDIALOG_H

#include <QDialog>

class UserPageWidget;
class QTextEdit;

class UserAccountsWidget : public QDialog
{
    Q_OBJECT
public:
    UserAccountsWidget(QWidget* parnet=0);
    void setCurrentUsers(QStringList currentUsers);
    QStringList currentUsers();
private slots:
    void saveButtonClicked();
signals:
    void logResult(QString string);
private:
    void insertUsers(QStringList usersToInsert);
    void deleteUsers(QStringList userstoDelete);
    UserPageWidget* userPageWidget;
};

class LogDialog : public QDialog
{
    Q_OBJECT
public:
    LogDialog(QWidget* parent = 0);
private slots:
    void log(QString string);
private:
    QTextEdit* textEdit;
    QPushButton* okButton;
};

#endif // USERACCOUNTSDIALOG_H
