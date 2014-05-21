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
