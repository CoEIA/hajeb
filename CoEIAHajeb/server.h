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
