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
#include <QtSql>
#include <QTimer>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include "processmonitor.h"
#include "../Global/global.h"

ProcessMonitor::ProcessMonitor(QObject *parent):
        QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeout()));
    m_timer->start();
}

void ProcessMonitor::timeout() {
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

    // Check if Process monitor is enabled for this user.
    if (!isEnabledForUser(userId)) {
        qDebug() << "Process monitor is disabled for this account.";
        return ;
    }

    QStringList pKillNames = getProcessName(userId);

    if (pKillNames.isEmpty())
        return ;

    m_process = new QProcess(this);

    foreach (QString pKillName,pKillNames) {
        QString pName = QString("taskkill /im %1 /f").arg(pKillName);
        m_process->start(pName);
        if (!m_process->waitForStarted())
            qDebug() << "can not start pskill process";
        if (!m_process->waitForFinished())
            qDebug() << "pskill process doesn't finished";
        QString output = m_process->readAll();
        qDebug() << output;
        if (output.contains("SUCCESS")) {
            QString action = "تم غلق البرنامج : " + pKillName + " بنجاح.";
            qDebug() << action ;
            logs(userId,action);
        }
    }

    delete m_process;
    m_process = 0;
}

QStringList ProcessMonitor::getProcessName(int userId) {
    QStringList pList;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT processName from LockedProgramsTable WHERE userId = ?");
    query.addBindValue(userId);
    query.exec();
    while (query.next()) {
        pList << query.value(0).toString();
    }
    return pList;
}

void ProcessMonitor::logs(int userId,QString action) {
    qDebug() << "User Id: " << userId;

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("INSERT INTO LogsTable(userId,action,currentDate,currentTime,actionType) VALUES (?,?,?,?,?);");
    query.addBindValue(userId);
    query.addBindValue(action);
    query.addBindValue(QDate::currentDate());
    query.addBindValue(QTime::currentTime());
    query.addBindValue(2);
    query.exec();
}

QString ProcessMonitor::getCurrentUserName(int userId) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT name from UsersTable WHERE userId = ?");
    query.addBindValue(userId);
    query.exec();
    query.next();
    return query.value(0).toString();
}


bool ProcessMonitor::isEnabledForUser(int id) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT programLock from SettingsTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    return (query.value(0).toInt()?true:false);
}

int ProcessMonitor::getCurrentUserId() {
    int userId;
    QFile file(USER_ID_FILE_PATH);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    stream >> userId;
    file.close();
    return userId;
}

int ProcessMonitor::getCurrentUserId(QString userName) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT userId from UsersTable WHERE name = ?");
    query.addBindValue(userName.trimmed());
    query.exec();
    if (query.next())
        return query.value(0).toInt();
    else
        return -1;
}

QString ProcessMonitor::getCurrentUserName() {
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
