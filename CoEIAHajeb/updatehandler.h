#ifndef UPDATEHANDLER_H
#define UPDATEHANDLER_H

class UpdateHandler
{
public:
    enum Response {
        UPDATE,
        NO_UPDATE,
        CONNECTION_ERROR
    };

    UpdateHandler();
    Response checkForUpdate();
    QStringList newFilesToUpdate();
private:
    QString readVersionFile();
    QStringList diff(QStringList newVersionStr,QStringList versionStr);
    QStringList filesToUpdate;
};

#endif // UPDATEHANDLER_H
