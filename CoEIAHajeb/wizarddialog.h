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
