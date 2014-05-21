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
