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
#include <QTextCodec>
#include "widget.h"
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
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(CoEIANetMonitor);
    a.setLayoutDirection(Qt::RightToLeft);

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

    Widget w;
    w.show();

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
