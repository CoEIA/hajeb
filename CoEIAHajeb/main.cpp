/*
** Unfortunately the code is bad (written without Object-oriented design) and it is not refactored due to the time limitation ,
** you will find a lot of duplicated code , legacy un-tested code.
**
** This project is start as proof-of-concept and to find a solution for HTTP blocking.
**
** Ahmed Isam <suda.nix@hotmail.com>
*/

#include <QtGui>
#include <QtSql/QtSql>

#ifdef Q_OS_WIN32
    #include <windows.h>
#endif

#include "mainwindow.h"
#include "wizarddialog.h"
#include "logindialog.h"
#include "../Global/global.h"
#include "useraccounts.h"
#include "database.h"
#include "applicationstatus.h"
#include "hajebservices.h"
#include "../Global/keyvalidator.h"
#include "server.h"

bool isDatabaseAvailable();
void loadStylesheet();
void setFilePath();
bool verifyKey(QString serialNumber);

QString INSTALL_PATH;
QString SHARED_PATH;
QString DB_PATH;
QString LSPDB_PATH;
QString CONNECTCMD_FILE_PATH;
QString USER_ID_FILE_PATH;
QString SCREENSHOTS_PATH;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setLayoutDirection(Qt::RightToLeft);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // Set application's file path
    setFilePath();

    // load style sheet
    loadStylesheet();

    // Check Application status
    ApplicationStatus applicationStatus;
    ApplicationStatus::Status status = applicationStatus.applicationStatus();
    if (status == ApplicationStatus::BLOCK || status == ApplicationStatus::END_TRIAL) {
//        QString currentUserName = getCurrentUserName();
//        if (currentUserName.toLower().trimmed() != "administrator" ) {
//            QMessageBox::critical(0,"انتهت الفترة التجريبية","لقد انتهت الفترة التجريبية ، قم بالدخول الى حساب المدير وادخال رقم تسجيل حاجب");
//            return -1 ;
//        }

        // enter key, then register app.
        bool ok;
        QString text = QInputDialog::getText(0, "لقد انتهت الفترة التجريبية","أدخل رقم تسجيل حاجب", QLineEdit::Normal,"", &ok);
        if (!ok || text.isEmpty())
            return -1;

        if (!verifyKey(text))
            return -1;
    }

    bool isDBAvailable = false;
    if (isDatabaseAvailable())
        isDBAvailable = true;

    Database db;

    if (!db.open()) {
        QMessageBox::warning(0,"Connection Error",QString("Cannot connect to Database.\n Please make sure that the database is already installed correctly."));
        return false;
    }

    if (!isDBAvailable) {
        QString currentUserName = getCurrentUserName();
        QStringList adminNames = getAllAdminsName();
        if (!adminNames.contains(currentUserName)) {
            QMessageBox::critical(0,"لا توجد لديك صلاحيات كافية لتشغيل معالج حاجب التمهيدي","لا يمكن تشغيل معالج حاجب التهميدي في حساب هذا المستخدم.\nيجب تسجيل الدخول الى النظام بحساب المدير وفي حالة عدم توفر حساب مدير افتراضي قم بتشغيل برنامج مدير حساب النظام المرفق مع حاجب وتفعيل حساب المدير.");
            return -1 ;
        }

        WizardDialog* wizard = new WizardDialog;
        wizard->exec();
    }

    if (!isDatabaseAvailable()) {
        QMessageBox::warning(0,"لا يمكن فتح البرنامج","قم باعادة تشغيل حاجب مجددا واكمال خطوات المعالج.\nلاحظ انه لا يمكن الدخول الى البرنامج دون انتهاء عمل المعالج بشكل صحيح");
        return -1;
    }

    if (a.argc() > 1) {
        if (a.arguments().at(1) == "-auto")
            HajebServices::install();
    }

    // Disable Hajeb in Windows XP (on user account).
    QString currentUserName = getCurrentUserName();
    QStringList adminNames = getAllAdminsName();
    if (!adminNames.contains(currentUserName)) {
        QMessageBox::critical(0,"لا يمكن تشغيل البرنامج","لا يمكن تشغيل حاجب في هذا الحساب ، قم بتشغيل البرنامج من حساب مدير للنظام");
        return -1 ;
    }

    LoginDialog loginDlg;
    if (loginDlg.exec() != QDialog::Accepted)
        return -1;

    MainWindow w;
    QObject::connect(&w,SIGNAL(updateCurrentDateTime()),&applicationStatus,SLOT(updateCurrentTimeDate()));
    w.show();

    return a.exec();
}

bool isDatabaseAvailable() {
    QFile dbFile(DB_PATH);
    if (dbFile.exists() && dbFile.size())
        return true;
    return false;
}

void loadStylesheet() {
    QFile file(":/CoEIAHajeb.qss");
    file.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(StyleSheet);
}

void setFilePath() {
    if (QSysInfo::windowsVersion() == QSysInfo::WV_XP || QSysInfo::windowsVersion() == QSysInfo::WV_2003)
        SHARED_PATH = SHARED_PATH_WINDOWS_XP;
    else if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA)
        SHARED_PATH = SHARED_PATH_WINDOWS_VIST;
    else
        SHARED_PATH = SHARED_PATH_WINDOWS_7;

    QDir dir;
    dir.cdUp();
    INSTALL_PATH = dir.toNativeSeparators(dir.path());

    // Files PATH
    DB_PATH = INSTALL_PATH + "\\" + PCSDB_NAME;
    LSPDB_PATH = INSTALL_PATH + "\\" + LSPDB_NAME;
    CONNECTCMD_FILE_PATH = INSTALL_PATH + "\\connectcmd.txt";
    USER_ID_FILE_PATH = INSTALL_PATH + "\\userId.txt";
    SCREENSHOTS_PATH = INSTALL_PATH + "\\Screenshots";
}

bool verifyKey(QString serialNumber) {
    if (KeyValidator::validate(serialNumber)) {
        KeyType state = Server::verifyKey(serialNumber);
        if (state == CLEAN_KEY) {
            if (Server::registerKeyOnline(serialNumber)) {
                QMessageBox::information(0,"تم التسجيل بنجاح","لقد تم تسجيل نسخة حاجب");
                ApplicationStatus::registerApplication(serialNumber,true);
                return true;
            } else
                QMessageBox::warning(0,"لم يتم تسجيل نسخة حاجب","لقد فشلت عملية تسجيل حاجب نرجوا المحاولة مجددا.");

        } else if (state == USED_KEY || state == ILLEGAL_KEY) {
            QMessageBox::warning(0,"لا يمكن تسجيل نسخة حاجب","عفوا ، رقم التسجيل غير صحيح.\nتأكد من ادخال الرقم بشكل صحيح.");

        } else if (state == UNKNOWN_KEY) {
            QMessageBox::information(0,"لا يوجد اتصال بالانترنت","يجب التأكد من وجود اتصال مع الانترنت ");
            //ApplicationStatus::registerApplication(serialNumber,false);
        }

    } else
        QMessageBox::warning(0,"لا يمكن تسجيل نسخة حاجب","عفوا ، رقم التسجيل غير صحيح.\nتأكد من ادخال الرقم بشكل صحيح.");

    return false;
}
