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
#include <QFile>
#include <QStringList>
#include "updatehandler.h"
#include "server.h"

UpdateHandler::UpdateHandler()
{
}

UpdateHandler::Response UpdateHandler::checkForUpdate() {
    QString newVersionStr = Server::getVersionNumberFile();
    if (newVersionStr == "ERORR")
        return CONNECTION_ERROR;
    QString versionStr = readVersionFile();

    filesToUpdate = diff(newVersionStr.split("\n"),versionStr.split("\n"));
    if (!filesToUpdate.count())
        return NO_UPDATE;
    else
        return UPDATE;
}

QString UpdateHandler::readVersionFile() {
    QFile file("version.txt");
    if (!file.open(QIODevice::ReadOnly))
        return QString();
    return QString(file.readAll());
}

QStringList UpdateHandler::newFilesToUpdate() {
    return filesToUpdate;
}

QStringList UpdateHandler::diff(QStringList newVersionStr,QStringList versionStr) {
    QStringList diffList;
    for (int i=0; i<newVersionStr.count(); ++i) {
        if (newVersionStr[i] != versionStr[i])
            diffList << newVersionStr[i].split(" ")[0];
    }

    return diffList;
}
