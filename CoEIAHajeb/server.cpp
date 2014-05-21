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
#include <QDebug>

#include "server.h"
#include "serverhandler.h"
#include "constant.h"

KeyType Server::verifyKey(QString key) {
    ServerHandler* handler = new ServerHandler;
    QString state = handler->verifyKey(key);
    if (state.contains("OK"))
        return CLEAN_KEY;
    else if (state.contains("Used"))
        return USED_KEY;
    else if (state.contains("NO"))
        return ILLEGAL_KEY;
    else
        return UNKNOWN_KEY;
}

bool Server::registerKeyOnline(QString key) {
    ServerHandler* handler = new ServerHandler;
    QString state = handler->registerKeyOnline(key,"");
    if (state == "DONE")
        return true;
    else {
        qDebug() << state;
        return false;
    }
}

bool Server::checkForUpdate() {

}

QString Server::getVersionNumberFile() {
    ServerHandler* handler = new ServerHandler;
    return handler->getVersionNumberFile();
}


