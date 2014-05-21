#include <QtCore/QCoreApplication>
#include <QtSql>
#include "processmonitor.h"
#include "../Global/global.h"

QString INSTALL_PATH;
QString SHARED_PATH ;
QString DB_PATH;
QString CONNECTCMD_FILE_PATH;
QString USER_ID_FILE_PATH;
QString SCREENSHOTS_PATH;
bool establishConnection();


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());

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
    CONNECTCMD_FILE_PATH = INSTALL_PATH + "\\connectcmd.txt";
    USER_ID_FILE_PATH = INSTALL_PATH + "\\userId.txt";
    SCREENSHOTS_PATH = INSTALL_PATH + "\\Screenshots";

    if (!establishConnection())
        return -1;

    ProcessMonitor pm;

    return a.exec();
}

bool establishConnection() {
    if (!QFile::exists(DB_PATH))
        return false;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(DB_PATH);
    db.setUserName("");

    if (!db.open()) {
        qDebug() << "Cannot open database";
        return false;
    }

    return true;
}
