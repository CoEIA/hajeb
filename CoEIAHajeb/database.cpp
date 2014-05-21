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
