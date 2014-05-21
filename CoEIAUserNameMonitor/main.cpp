#include <QtCore/QCoreApplication>
#include <QFile>
#include <QtSql>
#include <QDebug>
#include <QProcess>
#include <windows.h>
#include "../Global/global.h"

QString INSTALL_PATH;
QString SHARED_PATH ;
QString DB_PATH;
QString CONNECTCMD_FILE_PATH;
QString USER_ID_FILE_PATH;
QString SCREENSHOTS_PATH;

bool establishConnection();
int getCurrentUserId(QString userName);
void sendMessage();
void writeId();
void setPath();


TCHAR szName[]=TEXT("Global\\LSPSHMM");


#define SIG_NO_DATA		0
#define SIG_DATA		1
#define DOMAIN_SIZE		100
#define BUFF_SIZE		4000
#define URL_SIZE		3000

enum MSG_TYPE { USER_LOG, WEB_LOG };

struct MySharedData
{
    unsigned char flag;
    MSG_TYPE type;
    int userId;
    unsigned char domain[DOMAIN_SIZE];
    unsigned char url[URL_SIZE];
    bool log;
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    setPath();

    if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA || QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7) {
        // Start Windows 7 Services by starting customeMessageSender
        sendMessage();
    } else {
        // if Windows XP, write user id in shared file.
        if (!establishConnection()) {
            return -1;
        }
        writeId();
    }

    return 0;
}

bool establishConnection() {
    if (!QFile::exists(DB_PATH))
        return false;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(DB_PATH);
    db.setUserName("");

    if (!db.open()) {
        qDebug() << "Cannot open database.";
        return false;
    }

    return true;
}

int getCurrentUserId(QString userName) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT userId from UsersTable WHERE name = ?");
    query.addBindValue(userName);
    query.exec();
    if (query.next())
        return query.value(0).toInt();
    else
        return -1;
}

void sendMessage() {
    QProcess* m_process = new QProcess;
    QString pName = "\"CustomMessageSender.exe\"";
    m_process->start(pName);
    m_process->waitForStarted();
    m_process->waitForFinished();
    delete m_process;
    m_process = 0;
}

void writeId() {
    TCHAR buffer[100];
    DWORD buffer_size = 100;
    GetUserName(buffer,&buffer_size);
    QString userName = QString::fromWCharArray(buffer);

    int userId = getCurrentUserId(userName);

    // write userId to shared memory.
    HANDLE hMapFile;
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,szName);
    if (hMapFile == NULL)
    {
        //_tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
        return ;
    }

    MySharedData* pSharedData = (MySharedData *) MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,BUFF_SIZE);

    HANDLE hMutex;
    hMutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,TEXT("Global\\MyMutexObject"));
    if (hMutex == NULL) {
        //printf("OpenMutex error: %d\n", GetLastError() );
        return ;
    }

    // acquire the mutex.
    WaitForSingleObject(hMutex,INFINITE);

    // set data
    pSharedData->type = USER_LOG;
    pSharedData->userId = userId;
    CopyMemory(pSharedData->domain, "0", 1);    // not important here.
    CopyMemory(pSharedData->url, "0", 1);
    pSharedData->log = false;

    // set flag
    pSharedData->flag = SIG_DATA;

    // release the mutex.
    ReleaseMutex(hMutex);


    CloseHandle(hMutex);
    UnmapViewOfFile(pSharedData);
    CloseHandle(hMapFile);

//    QFile file(USER_ID_FILE_PATH);
//    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
//        qDebug() << "File open error: " << file.errorString();
//    } else {
//        QTextStream stream(&file);
//        stream << userId;
//        file.close();
//    }

//    qDebug() << "Welcom: " << userName;
//    qDebug() << "Id: " << userId;
}

void setPath() {
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
}
