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
#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QString>
#include <QMap>
#include "constant.h"

class Server
{
public:
    static KeyType verifyKey(QString key);
    static bool registerKeyOnline(QString key);
    static QString getVersionNumberFile();
    static bool checkForUpdate();
};

#endif // SERVER_H
