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
#include "database.h"
#include "../Global/global.h"

Database::Database()
{
    // Create Default DB for Hajeb
    QSqlDatabase::addDatabase("QSQLITE",PCSDB_NAME);
    // Create LSPDB
    QSqlDatabase::addDatabase("QSQLITE",LSPDB_NAME);
}

bool Database::open() {
    QSqlDatabase defaultDB = QSqlDatabase::database(PCSDB_NAME,false);
    defaultDB.setHostName("localhost");
    defaultDB.setDatabaseName(DB_PATH);
    defaultDB.setUserName("");
    if (!defaultDB.open())
        return false;

    QSqlDatabase LSPDB = QSqlDatabase::database(LSPDB_NAME,false);
    LSPDB.setHostName("localhost");
    LSPDB.setDatabaseName(LSPDB_PATH);
    LSPDB.setUserName("");
    if (!LSPDB.open())
        return false;
    return true;
}
