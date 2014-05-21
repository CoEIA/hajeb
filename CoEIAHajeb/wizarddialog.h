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
#ifndef WIZARDDIALOG_H
#define WIZARDDIALOG_H

#include <QDialog>
#include <QWidget>

class SlidingStackedWidget;
class QPushButton;
class QListWidget;
class QTextEdit;
class QComboBox;
class QLineEdit;
class QCheckBox;

class IntroductionPageWidget;
class UserPageWidget;
class ServicesPageWidget;
class FinishPageWidget;
class PasswordPageWidget;

class WizardDialog : public QDialog
{
    Q_OBJECT
public:
    enum PAGES {
        IntroductionPage,
        UserAccountsPage,
        ServicesPage,
        PasswordPage,
        FinishPage
    };
    WizardDialog(QWidget* parent=0);
private slots:
    void nextClicked();
    void previousClicked();
private:
    void writeSettings();
    IntroductionPageWidget* introductionPageWidget;
    UserPageWidget* userPageWidget;
    ServicesPageWidget* servicesPageWidget;
    PasswordPageWidget* passwordPageWidget;
    FinishPageWidget* finishPageWidget;
    SlidingStackedWidget* stackedWidget;
    QPushButton* nextButton;
    QPushButton* previousButton;
    PAGES m_currentPage;
};


class IntroductionPageWidget : public QWidget
{
public:
    IntroductionPageWidget(QWidget* parent=0);
};

class UserPageWidget : public QWidget
{
    Q_OBJECT
public:
    UserPageWidget(QWidget* parent=0);
    QStringList userAccounts();
    void setCurrentUsers(QStringList);
public slots:
    void addUser();
    void removeUser();
protected:
    void initAllUserListWidget();
    QStringList users;
    QListWidget* allUserListWidget;
    QListWidget* choosedUserListWidget;
};

class ServicesPageWidget : public QWidget
{
    Q_OBJECT
public:
    ServicesPageWidget(QWidget* parent=0);
    int programLock() const ;
    int websiteLock() const ;
    int browseTime() const ;
    int monitorBrowse() const;
    int monitorKeyboard() const;
    int monitorScreen() const;
private:
    QCheckBox* programMonitorCheckBox;
    QCheckBox* websiteMonitorCheckBox;
    QCheckBox* browseTimeCheckBox;
    QCheckBox* browseMonitorCheckBox;
    QCheckBox* keyMonitorCheckBox;
    QCheckBox* screenshotMonitorCheckBox;
};

class PasswordPageWidget : public QWidget
{
    Q_OBJECT
public:
    PasswordPageWidget(QWidget* parent=0);
    void setUsersName(QStringList users);
    QString password();
    QString confirmPassword();
    QString root();
private:
    QString m_root;
    QComboBox* moderatorComboBox;
    QLineEdit* passwordLineEdit;
    QLineEdit* confirmPasswordLineEdit;
};

class FinishPageWidget : public QWidget
{
public:
    FinishPageWidget(QWidget* parent=0);
    void logResult(QString result);
private:
    QTextEdit* wizardFinishPageTextEdit;
};

#endif // WIZARDDIALOG_H
