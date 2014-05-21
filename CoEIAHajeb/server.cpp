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


