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
#include <QSqlQuery>
#include "screenshotmonitor.h"
#include "../Global/global.h"

ScreenshotMonitor::ScreenshotMonitor(QObject *parent):
        QObject(parent)
{
    m_screenshotTimePerdiod =  new QTimer(this);
    m_screenshotTimePerdiod->setInterval(5*1000);
    connect(m_screenshotTimePerdiod,SIGNAL(timeout()),this,SLOT(readTimePeriod()));
    m_screenshotTimePerdiod->start();

    m_timer = new QTimer(this);
    m_timer->setInterval(5*60*60*1000); // = 5 Hours.
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeout()));
    m_timer->start();
    qDebug() << "Wait timout...";
}

void ScreenshotMonitor::timeout() {
    int id;

    if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA || QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7) {
        QString userName = getCurrentUserName();
        id = getCurrentUserId(userName);
    } else {
        id = getCurrentUserId();
    }

    // Check if current user is controlled by Hajeb
    if (id == -1) {
        return ;
    }

    QString userName = getCurrentUserName(id);
    qDebug() << "userName" << userName ;

    // Check if screenshot monitor is enabled for this user.
    if (!isEnabledForUser(id)) {
        qDebug() << "Screenshot is disable for this account.";
        return ;
    }

    // Check if Screenshots Dir is Avaiable.
    QDir screenshotsDir(INSTALL_PATH);
    if (!screenshotsDir.exists("Screenshots")) {
        if (!screenshotsDir.mkdir("Screenshots"))
            qDebug() << "Unable to create Screenshots directory";
    }

    QString imagePath = SCREENSHOTS_PATH;
    QDir userNameDir(imagePath);
    if (!userNameDir.exists(imagePath + "\\" + userName)) {
        if (!userNameDir.mkdir(userName))
            qDebug() << "Unable to create directory";
    }

    userNameDir.cd(userName);
    qDebug() << "Absolute Path: " << userNameDir.absolutePath();
    int size = userNameDir.entryInfoList(QDir::Files).count();
    size++;
    QString filePath = userNameDir.absolutePath() + "/" + QString::number(size) + ".png";
    qDebug() << "File Path: " << filePath;
    QPixmap screenshot = QPixmap::grabWindow(QApplication::desktop()->winId());
    bool isSaved = screenshot.save(filePath);
    if (!isSaved)
        qDebug() << "Can not save screenshot";
    else
        qDebug() << "Screenshot saved.";
}


void ScreenshotMonitor::readTimePeriod() {
    int userId;

    if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA || QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7) {
        QString userName = getCurrentUserName();
        userId = getCurrentUserId(userName);
    } else {
        userId = getCurrentUserId();
    }

    if (userId == -1)
        return ;

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT timePeriod from ScreenshotsTable WHERE userId = ?");
    query.addBindValue(userId);
    query.exec();
    query.next();
    int time =  query.value(0).toInt();
    qDebug() << "Time in DB: " << time ;
    qDebug() << "Timers: " << m_timer->interval()/60000;

    if (m_timer->interval() == time*60*1000)
        return ;

    if (m_timer->isActive())
        m_timer->stop();
    m_timer->setInterval(time*60*1000);
    m_timer->start();
}

QString ScreenshotMonitor::getCurrentUserName(int userId) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT name from UsersTable WHERE userId = ?");
    query.addBindValue(userId);
    query.exec();
    query.next();
    return query.value(0).toString();
}


bool ScreenshotMonitor::isEnabledForUser(int id) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT monitorScreen from SettingsTable WHERE userId = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    return (query.value(0).toInt()?true:false);
}

int ScreenshotMonitor::getCurrentUserId() {
    int userId;
    QFile file(USER_ID_FILE_PATH);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    stream >> userId;
    file.close();
    return userId;
}

int ScreenshotMonitor::getCurrentUserId(QString userName) {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT userId from UsersTable WHERE name = ?");
    query.addBindValue(userName.trimmed());
    query.exec();
    if (query.next())
        return query.value(0).toInt();
    else
        return -1;
}

QString ScreenshotMonitor::getCurrentUserName() {
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
