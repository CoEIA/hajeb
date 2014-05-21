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
#include <QtGui>
#include <QtSql>
#include <QCryptographicHash>

#ifdef Q_OS_WIN32
    #include <windows.h>
    #include <string>
    #include "resettingregistry.h"
#endif
#include "wizarddialog.h"
#include "hajebservices.h"
#include "useraccounts.h"
#include "SlidingStackedWidget.h"
#include "../Global/global.h"


IntroductionPageWidget::IntroductionPageWidget(QWidget *parent):
        QWidget(parent)
{
    QLabel* welcomeLabel = new QLabel;
    welcomeLabel->setText("<b>مرحبا بكم في نظام حاجب.\nسيقوم المعالج التمهيدي بتثبيت قاعدة البيانات وإعداد النظام للعمل\nقم بالضغط على زر التالي الموجود في الأسفل للإنتقال الى شاشة إعداد المستخدمين.</b>");
    welcomeLabel->setWordWrap(true);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(welcomeLabel);
    this->setWindowTitle("معالج حاجب التمهيدي");
}

UserPageWidget::UserPageWidget(QWidget *parent):
        QWidget(parent)
{
    users.clear();
    QLabel* label = new QLabel;
    label->setWordWrap(true);
    label->setText("  قم باختيار المستخدمين المراد تطبيق خدمات حاجب عليهم ، ننصح باختيار جميع المستخدمين وعدم اختيار الأجهزة الوهمية. ويمكن لاحقا اضافة او الزالة أي مستخدم.");
    QGroupBox* groupBox = new QGroupBox;
    allUserListWidget = new QListWidget;
    allUserListWidget->setObjectName("allUserListWidget");
    allUserListWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    choosedUserListWidget = new QListWidget;
    choosedUserListWidget->setObjectName("choosedUserListWidget");
    choosedUserListWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    QLabel* allUserLabel = new QLabel("كل المستخدمين الموجودين على الحاسب");
    QLabel* choosedUserLabel = new QLabel("المستخدمين الذي تم اختيارهم");
    QToolButton* insertButton = new QToolButton;
    insertButton->setText(">>");
    insertButton->setToolTip("اضافة مستخدم");
    QToolButton* removeButton = new QToolButton;
    removeButton->setText("<<");
    removeButton->setToolTip("حذف مستخدم");

    connect(insertButton,SIGNAL(clicked()),this,SLOT(addUser()));
    connect(removeButton,SIGNAL(clicked()),this,SLOT(removeUser()));

    QVBoxLayout* allUserLayout = new QVBoxLayout;
    allUserLayout->addWidget(allUserLabel);
    allUserLayout->addWidget(allUserListWidget);
    QVBoxLayout* choosedUserLayout = new QVBoxLayout;
    choosedUserLayout->addWidget(choosedUserLabel);
    choosedUserLayout->addWidget(choosedUserListWidget);
    QVBoxLayout* toolButtonLayout = new QVBoxLayout;
    toolButtonLayout->addWidget(insertButton);
    toolButtonLayout->addWidget(removeButton);
    QHBoxLayout* groupLayout = new QHBoxLayout(groupBox);
    groupLayout->addLayout(allUserLayout);
    groupLayout->addLayout(toolButtonLayout);
    groupLayout->addLayout(choosedUserLayout);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(groupBox);

    initAllUserListWidget();
    this->setWindowTitle("إعداد المستخدمين");
}

void UserPageWidget::addUser() {
    if (allUserListWidget->currentIndex().row() < 0 ) {
        QMessageBox::warning(this,"خطأ","قم باختيار أحد المستخدمين لكي تتم إضافته");
        return ;
    }

//    QString user = allUserListWidget->item(row)->text();
//    if (!users.contains(user)) {
//        choosedUserListWidget->addItem(new QListWidgetItem(QIcon(":/images/avatar.png"),user));
//        users.append(user);
//    } else
//        QMessageBox::warning(this,"خطأ","لا يمكن اختيار المستخدم أكثر من مرة واحدة");

    int row = allUserListWidget->currentIndex().row();
    users.append(allUserListWidget->item(row)->text());
    choosedUserListWidget->addItem(allUserListWidget->takeItem(row));
    choosedUserListWidget->clearSelection();
}

void UserPageWidget::removeUser() {
    if (choosedUserListWidget->currentIndex().row() < 0 ) {
        QMessageBox::warning(this,"خطأ","قم باختيار أحد المستخدمين لكي يتم حذفه");
        return ;
    }

//    QListWidgetItem* item = choosedUserListWidget->item(choosedUserListWidget->currentIndex().row());
//    users.removeAll(item->text());
//    choosedUserListWidget->clear();
//    foreach (QString user,users)
//        choosedUserListWidget->addItem(new QListWidgetItem(QIcon(":/images/avatar.png"),user));

    int row = choosedUserListWidget->currentIndex().row();
    users.removeAll(choosedUserListWidget->item(row)->text());
    allUserListWidget->addItem(choosedUserListWidget->takeItem(row));
    choosedUserListWidget->clearSelection();
}

void UserPageWidget::initAllUserListWidget() {
    foreach (QString user,getAllUserAccounts())
        allUserListWidget->addItem(new QListWidgetItem(QIcon(":/images/avatar.png"),user));
}

QStringList UserPageWidget::userAccounts() {
    return users;
}

void UserPageWidget::setCurrentUsers(QStringList currentUsers) {
    users.clear();
    users = currentUsers;
    choosedUserListWidget->clear();
    allUserListWidget->clear();

    foreach (QString user,getAllUserAccounts()) {
        if (users.contains(user))
            choosedUserListWidget->addItem(new QListWidgetItem(QIcon(":/images/avatar.png"),user));
        else
            allUserListWidget->addItem(new QListWidgetItem(QIcon(":/images/avatar.png"),user));
    }
//    foreach (QString user,users) {
//        choosedUserListWidget->addItem(new QListWidgetItem(QIcon(":/images/avatar.png"),user));
//        QListWidgetItem* item = allUserListWidget->findItems(user,Qt::MatchExactly)[0];
//        allUserListWidget->removeItemWidget();
//    }
//    choosedUserListWidget->clearSelection();
}

ServicesPageWidget::ServicesPageWidget(QWidget *parent):
        QWidget(parent)
{
    programMonitorCheckBox = new QCheckBox;
    programMonitorCheckBox->setText("خدمة حجب البرامج");
    programMonitorCheckBox->setChecked(true);
    websiteMonitorCheckBox = new QCheckBox;
    websiteMonitorCheckBox->setText("خدمة حجب المواقع");
    websiteMonitorCheckBox->setChecked(true);
    browseTimeCheckBox = new QCheckBox;
    browseTimeCheckBox->setText("خدمة تحديد زمن تصفح الإنترنت");
    browseTimeCheckBox->setChecked(true);
    browseMonitorCheckBox = new QCheckBox;
    browseMonitorCheckBox->setText("خدمة مراقبة التصفح");
    browseMonitorCheckBox->setChecked(true);
    keyMonitorCheckBox = new QCheckBox;
    keyMonitorCheckBox->setText("خدمة مراقبة لوحة المفاتيح");
    keyMonitorCheckBox->setChecked(true);
    screenshotMonitorCheckBox = new QCheckBox;
    screenshotMonitorCheckBox->setText("خدمة مراقبة الشاشة");
    screenshotMonitorCheckBox->setChecked(true);
    QLabel* serviceLabel = new QLabel;
    serviceLabel->setText("قم بتحديد خدمات حاجب التي ترغب بتفعيلها الان. ويمكنك لاحقا اضافة او الزالة الخدمات من لوحة تحكم البرنامج.\nننصح بتفعيل جميع خدمات حاجب");

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(programMonitorCheckBox,0,0);
    gridLayout->addWidget(websiteMonitorCheckBox,0,1);
    gridLayout->addWidget(browseTimeCheckBox,0,2);
    gridLayout->addWidget(browseMonitorCheckBox,1,0);
    gridLayout->addWidget(keyMonitorCheckBox,1,1);
    gridLayout->addWidget(screenshotMonitorCheckBox,1,2);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(serviceLabel);
    layout->addLayout(gridLayout);
    layout->addStretch();
    this->setWindowTitle("إعداد خدمات حاجب");
}

int ServicesPageWidget::programLock() const {
    return (programMonitorCheckBox->isChecked()?1:0);
}
int ServicesPageWidget::websiteLock() const {
    return (websiteMonitorCheckBox->isChecked()?1:0);
}
int ServicesPageWidget::browseTime() const {
    return (browseTimeCheckBox->isChecked()?1:0);
}
int ServicesPageWidget::monitorBrowse() const {
    return (browseMonitorCheckBox->isChecked()?1:0);
}
int ServicesPageWidget::monitorKeyboard() const {
    return (keyMonitorCheckBox->isChecked()?1:0);
}
int ServicesPageWidget::monitorScreen() const {
    return (screenshotMonitorCheckBox->isChecked()?1:0);
}

PasswordPageWidget::PasswordPageWidget(QWidget *parent):
        QWidget(parent)
{
    moderatorComboBox = new QComboBox;
    moderatorComboBox->addItems(QStringList());
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit = new QLineEdit;
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

    QLabel* moderatorNameLabel = new QLabel;
    moderatorNameLabel->setText("اسم مدير برنامج حاجب");
    QLabel* moderatorPasswordLabel = new QLabel;
    moderatorPasswordLabel->setText("انشاء كلمة مرور لحاجب");
    QLabel* iconLabel = new QLabel;
    iconLabel->setPixmap(QPixmap(":/images/Administrator.png"));
    QLabel* confirmPasswordLabel = new QLabel;
    confirmPasswordLabel->setText("تأكيد كلمة المرور");

    moderatorComboBox->addItem(getCurrentUserName());

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(moderatorNameLabel,0,0);
    gridLayout->addWidget(moderatorComboBox,0,1);
    gridLayout->addWidget(moderatorPasswordLabel,1,0);
    gridLayout->addWidget(passwordLineEdit,1,1);
    gridLayout->addWidget(confirmPasswordLabel,2,0);
    gridLayout->addWidget(confirmPasswordLineEdit,2,1);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(iconLabel);
    layout->addLayout(gridLayout);
    this->setWindowTitle("انشاء كلمة مرور لحاجب");
}

void PasswordPageWidget::setUsersName(QStringList users) {
    moderatorComboBox->clear();
    QStringList admins = getAllAdminsName();
    QStringList adminsUserName ;
    foreach (QString user,users) {
        if (admins.contains(user))
            adminsUserName << user;
    }

    moderatorComboBox->addItems(adminsUserName);
    moderatorComboBox->setCurrentIndex(0);
}

QString PasswordPageWidget::password() {
    return passwordLineEdit->text();
}

QString PasswordPageWidget::confirmPassword() {
    return confirmPasswordLineEdit->text();
}

QString PasswordPageWidget::root() {
    return moderatorComboBox->currentText();
}

FinishPageWidget::FinishPageWidget(QWidget *parent):
        QWidget(parent)
{
    wizardFinishPageTextEdit = new QTextEdit;
    QLabel* label = new QLabel;
    label->setText("عملية إعداد حاجب");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(wizardFinishPageTextEdit);
}

void FinishPageWidget::logResult(QString result) {
    wizardFinishPageTextEdit->append(result);
}

WizardDialog::WizardDialog(QWidget *parent):
        QDialog(parent),m_currentPage(IntroductionPage)
{
    // Check Admin Privileges.
//    QString currentUserName = getCurrentUserName();
//    QStringList adminNames = getAllAdminsName();
//    if (!adminNames.contains(currentUserName)) {
//        QMessageBox::critical(0,"لا يمكن تشغيل معالج حاجب","بسبب عدم وجود صلاحيات كافية فانه لا يمكن تشغيل معالج حاجب التمهيدي.\nفقط مدير النظام هو المصرح له بتشغيل هذا المعالج.");
//        return  ;
//    }

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedWidth(550);

    introductionPageWidget = new IntroductionPageWidget;
    userPageWidget = new UserPageWidget;
    servicesPageWidget = new ServicesPageWidget;
    passwordPageWidget = new PasswordPageWidget;
    finishPageWidget = new FinishPageWidget;

    stackedWidget = new SlidingStackedWidget(this);
    stackedWidget->addWidget(introductionPageWidget);
    stackedWidget->addWidget(userPageWidget);
    stackedWidget->addWidget(servicesPageWidget);
    stackedWidget->addWidget(passwordPageWidget);
    stackedWidget->addWidget(finishPageWidget);
    stackedWidget->setSpeed(500);

    QLabel* headerLabel = new QLabel;
    QPixmap pixmap(":/images/header.PNG");
    headerLabel->setPixmap(pixmap);

    nextButton = new QPushButton;
    nextButton->setText("التالي");
    nextButton->setIcon(QIcon(":/images/nextbutton.png"));
    connect(nextButton,SIGNAL(clicked()),this,SLOT(nextClicked()));

    previousButton = new QPushButton;
    previousButton->setText("السابق");
    previousButton->setIcon(QIcon(":/images/previousbutton.png"));
    previousButton->setEnabled(false);
    connect(previousButton,SIGNAL(clicked()),this,SLOT(previousClicked()));

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(previousButton);
    bottomLayout->addWidget(nextButton);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(headerLabel);
    layout->addWidget(stackedWidget);
    layout->addLayout(bottomLayout);

    this->setWindowTitle("نظام حاجب - مركز التميز لأمن المعلومات");
    this->setWindowIcon(QIcon(":/images/hajibIcon.ico"));
}

void WizardDialog::nextClicked() {
    if (m_currentPage == WizardDialog::IntroductionPage) {
        m_currentPage = WizardDialog::UserAccountsPage;
        this->setWindowTitle("إعداد المستخدمين");
    } else if (m_currentPage == WizardDialog::UserAccountsPage) {
        if (userPageWidget->userAccounts().isEmpty()) {
            QMessageBox::warning(this,"لا يمكن التقدم","من فضلك قم باختيار مستخدم واحد على الأقل.");
            return ;
        }
        m_currentPage = WizardDialog::ServicesPage;
        this->setWindowTitle("إعداد الخدمات");
    } else if (m_currentPage == WizardDialog::ServicesPage) {
        m_currentPage = WizardDialog::PasswordPage;
        this->setWindowTitle("كلمة مرور نظام حاجب");
        //passwordPageWidget->setUsersName(userPageWidget->userAccounts());
    } else if (m_currentPage == WizardDialog::PasswordPage) {
        if (passwordPageWidget->password().isEmpty()) {
            QMessageBox::warning(this,"لا يمكن التقدم","من فضلك قم بادخال كلمة المرور لنظام حاجب.");
            return ;
        }

        if (passwordPageWidget->password() != passwordPageWidget->confirmPassword()) {
            QMessageBox::warning(this,"لا يمكن التقدم","من فضلك قم بادخال كلمات مرور متطابقة.");
            return ;
        }

        m_currentPage = WizardDialog::FinishPage;
        nextButton->setText("إنهاء");
        this->setWindowTitle("نهاية الإعدادات");
    } else if (m_currentPage == WizardDialog::FinishPage) {
        this->close();
        return ;
    }

    if (m_currentPage == WizardDialog::IntroductionPage || m_currentPage == WizardDialog::FinishPage)
        previousButton->setEnabled(false);
    else
        previousButton->setEnabled(true);

    stackedWidget->slideInIdx(int(m_currentPage));
    if (m_currentPage == WizardDialog::FinishPage)
        writeSettings();
}

void WizardDialog::previousClicked() {
    if (m_currentPage == WizardDialog::UserAccountsPage) {
        m_currentPage = WizardDialog::IntroductionPage;
    } else if (m_currentPage == WizardDialog::ServicesPage) {
        m_currentPage = WizardDialog::UserAccountsPage;
    } else if (m_currentPage == WizardDialog::PasswordPage) {
        m_currentPage = WizardDialog::ServicesPage;

    }

    stackedWidget->slideInIdx(int(m_currentPage));
    this->setWindowTitle(stackedWidget->widget(int(m_currentPage))->windowTitle());
    if (m_currentPage == WizardDialog::IntroductionPage)
        previousButton->setEnabled(false);
    else
        previousButton->setEnabled(true);
}

void WizardDialog::writeSettings() {
    nextButton->setEnabled(false);
    qApp->processEvents();

    qDebug() << "Creating database...";
    finishPageWidget->logResult("إنشاء قاعدة البيانات ...");

    // Create PCS-DB tables.
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.exec("PRAGMA foreign_keys = ON;");    // Inforece foreing key constrain.
    query.exec("CREATE TABLE UsersTable (userId INTEGER PRIMARY KEY AUTOINCREMENT,name VARCHAR( 30 ) NOT NULL)");
    query.exec("CREATE TABLE SettingsTable (userId INTEGER PRIMARY KEY ,programLock INT NOT NULL ,websiteLock INT NOT NULL ,browseTime INT NOT NULL ,monitorBrowse INT NOT NULL ,monitorKeyboard INT NOT NULL ,monitorScreen INT NOT NULL , browseTimeMethod INT NOT NULL, foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE)");
    query.exec("CREATE TABLE KeylogsTable (no INTEGER PRIMARY KEY AUTOINCREMENT,userId INTEGER, programName VARCHAR( 50 ) NOT NULL,log TEXT ,currentDate DATE,currentTime TIME, foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE)");
    query.exec("CREATE TABLE LockedProgramsTable (no INTEGER PRIMARY KEY AUTOINCREMENT,userId INTEGER NOT NULL, processName VARCHAR( 50 ) NOT NULL,programName VARCHAR( 50 ) NOT NULL,lockDate DATE NOT NULL, foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE)");
    query.exec("CREATE TABLE LogsTable (no INTEGER PRIMARY KEY AUTOINCREMENT,userId INTEGER NOT NULL,action VARCHAR( 100 ) NOT NULL ,currentDate DATE,currentTime TIME,actionType INT NOT NULL,foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE,foreign key (actionType) references LogsActionTypeTable(actionType) ON DELETE CASCADE ON UPDATE CASCADE)");
    query.exec("CREATE TABLE LogsActionTypeTable (actionType INTEGER PRIMARY KEY AUTOINCREMENT,description VARCHAR( 100 ) NOT NULL)");
    query.exec("CREATE TABLE BrowseTimesTable (no INTEGER PRIMARY KEY AUTOINCREMENT,userId INTEGER NOT NULL,dayNo INTEGER NOT NULL ,timetable INTEGER NOT NULL ,foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE)");
    query.exec("CREATE TABLE BrowseTimesCounterTable (no INTEGER PRIMARY KEY AUTOINCREMENT,userId INTEGER NOT NULL,dayNo INTEGER NOT NULL ,allowedTime TIME , remainingTime TIME, lastUseDate DATE , foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE)");
    query.exec("CREATE TABLE ProgramAccountsTable (userId INTEGER PRIMARY KEY AUTOINCREMENT,password TEXT NOT NULL)");
    query.exec("CREATE TABLE ScreenshotsTable (userId INTEGER PRIMARY KEY AUTOINCREMENT,timePeriod INT NOT NULL DEFAULT 5 ,foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE)");

    query.exec("CREATE TABLE CategoriesTable (catId INTEGER PRIMARY KEY AUTOINCREMENT, catName VARCHAR (50) NOT NULL, userId INTEGER NOT NULL, state INT NOT NULL, type INT NOT NULL, foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE)");
    query.exec("CREATE TABLE WebsitesTable (no INTEGER PRIMARY KEY AUTOINCREMENT, url VARCHAR( 50 ) NOT NULL, host VARCHAR( 50 ) NOT NULL, catId INT NOT NULL, state INT NOT NULL, foreign key (catId) references CategoriesTable(catId) ON DELETE CASCADE ON UPDATE CASCADE)");


    query.exec("INSERT INTO LogsActionTypeTable(actionType,description) VALUES (1,\"WebSite Lock\") ;");
    query.exec("INSERT INTO LogsActionTypeTable(actionType,description) VALUES (2,\"Program Lock\") ;");

    qApp->processEvents();

    // Create LSP-DB tables.
    QSqlQuery LSPquery(QSqlDatabase::database(LSPDB_NAME));
    LSPquery.exec("PRAGMA foreign_keys = ON;");    // Inforece foreing key constrain.
    LSPquery.exec("CREATE TABLE UsersTable (userId INTEGER PRIMARY KEY AUTOINCREMENT,name VARCHAR( 30 ) NOT NULL, listType INT NOT NULL DEFAULT 0)"); /* 0 = black list, 1 = white list */
    LSPquery.exec("CREATE TABLE BrowseLogsTable (no INTEGER PRIMARY KEY AUTOINCREMENT,userId INTEGER NOT NULL,domain VARCHAR( 50 ) NOT NULL ,url VARCHAR( 200 ) NOT NULL ,currentDate DATE,currentTime TIME ,foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE)");
    LSPquery.exec("CREATE TABLE LockedWebsitesTable (no INTEGER PRIMARY KEY AUTOINCREMENT,userId INTEGER NOT NULL, url VARCHAR( 50 ) NOT NULL,host VARCHAR( 50 ) NOT NULL,lockDate DATE NOT NULL ,foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE)");
    LSPquery.exec("CREATE TABLE LogsActionTypeTable (actionType INTEGER PRIMARY KEY AUTOINCREMENT,description VARCHAR( 100 ) NOT NULL)");
    LSPquery.exec("CREATE TABLE LogsTable (no INTEGER PRIMARY KEY AUTOINCREMENT,userId INTEGER NOT NULL,action VARCHAR( 100 ) NOT NULL ,currentDate DATE,currentTime TIME,actionType INT NOT NULL,foreign key (userId) references UsersTable(userId) ON DELETE CASCADE ON UPDATE CASCADE,foreign key (actionType) references LogsActionTypeTable(actionType) ON DELETE CASCADE ON UPDATE CASCADE)");
    LSPquery.exec("INSERT INTO  LogsActionTypeTable(actionType,description) VALUES (1,\"WebSite Lock\") ;");
    LSPquery.exec("INSERT INTO  LogsActionTypeTable(actionType,description) VALUES (2,\"Program Lock\") ;");

    qApp->processEvents();

    QStringList users = userPageWidget->userAccounts();

    // Add users to PCS-DB
    foreach (QString user, users) {
        query.prepare("INSERT INTO UsersTable(name) VALUES (?) ;");
        query.addBindValue(user);
        if (query.exec())
            finishPageWidget->logResult(QString("إنشاء ملف للمستخدم: %1 ...").arg(user));
    }

    // Add users to LSP-DB
    foreach (QString user, users) {
        LSPquery.prepare("INSERT INTO UsersTable(name) VALUES (?) ;");
        LSPquery.addBindValue(user);
        LSPquery.exec();
    }

    qApp->processEvents();

    foreach (QString user, users) {
        query.prepare("SELECT userId FROM UsersTable WHERE name = ?");
        query.addBindValue(user);
        query.exec();
        query.next();
        int id = query.value(0).toInt();
        query.prepare("INSERT INTO SettingsTable(userId,programLock,websiteLock,browseTime,monitorBrowse,monitorKeyboard,monitorScreen,browseTimeMethod) VALUES (?,?,?,?,?,?,?,?) ;");
        query.addBindValue(id);
        query.addBindValue(servicesPageWidget->programLock());
        query.addBindValue(servicesPageWidget->websiteLock());
        query.addBindValue(servicesPageWidget->browseTime());
        query.addBindValue(servicesPageWidget->monitorBrowse());
        query.addBindValue(servicesPageWidget->monitorKeyboard());
        query.addBindValue(servicesPageWidget->monitorScreen());
        query.addBindValue(0);  /* 0 = TimeTable, 1 = Counter */
        query.exec();

        qApp->processEvents();

        for (int day=1; day < 8; day++) {
            query.prepare("INSERT INTO BrowseTimesTable(userId,dayNo,timetable) VALUES (?,?,16777215) ;");
            query.addBindValue(id);
            query.addBindValue(day);
            query.exec();
        }

        for (int day=1; day < 8; day++) {
            query.prepare("INSERT INTO BrowseTimesCounterTable(userId,dayNo,allowedTime,remainingTime,lastUseDate) VALUES (?,?,?,?,?) ;");
            query.addBindValue(id);
            query.addBindValue(day);
            query.addBindValue(QTime(23,59,59));
            query.addBindValue(QTime(23,59,59));
            query.addBindValue(QDate::currentDate());
            query.exec();
        }

        query.prepare("INSERT INTO ScreenshotsTable(userId,timePeriod) VALUES (?,300) ;");
        query.addBindValue(id);
        query.exec();


        // Block chrome.exe for all non-admin user.
        QStringList adminNames = getAllAdminsName();
        if (!adminNames.contains(user)) {
            query.prepare("INSERT INTO LockedProgramsTable(userId,processName,programName,lockDate) VALUES (?,?,?,?) ;");
            query.addBindValue(id);
            query.addBindValue("chrome.exe");
            query.addBindValue("chrome");
            query.addBindValue(QDate::currentDate());
            query.exec();
        }
    }


    qApp->processEvents();

    QString root = passwordPageWidget->root();
    QString password = passwordPageWidget->password();

    int userId ;
    qDebug() << "root: " << root;
    qDebug() << "pass: " << password;

    query.prepare("SELECT userId FROM UsersTable WHERE name = ?");
    query.addBindValue(root);
    query.exec();
    if (query.next())
        userId = query.value(0).toInt();
    else
        userId = -1 ;

    QString md5Pass = QString(QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Md5).toHex());

    query.prepare("INSERT INTO ProgramAccountsTable(userId,password) VALUES (?,?) ;");
    query.addBindValue(userId);
    query.addBindValue(md5Pass);
    qDebug() << query.exec();

    qApp->processEvents();



    QFile userIdFile(USER_ID_FILE_PATH);
    if (!userIdFile.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        qDebug() << "File open error: " << userIdFile.errorString();
    } else {
        QTextStream stream(&userIdFile);
        stream << userId;
        userIdFile.close();
    }

    QFile connectCmdFile(CONNECTCMD_FILE_PATH);
    if (!connectCmdFile.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        qDebug() << "File open error: " << connectCmdFile.errorString();
    } else {
        QTextStream stream(&connectCmdFile);
        stream << 1;
        connectCmdFile.close();
    }

    qApp->processEvents();

    createHajebPathKey();

    qApp->processEvents();

    HajebServices::install();
    finishPageWidget->logResult("<b>تم تشغيل خدمات حاجب</b>.");

    qApp->processEvents();

    bool isInstalled = HajebServices::installLSP();
    if (isInstalled)
        finishPageWidget->logResult("<b>تم تشغيل Hajeb Filter</b>.");

    qApp->processEvents();

    finishPageWidget->logResult("<b>تم الإعداد بنجاح</b>.");
    nextButton->setEnabled(true);
}

