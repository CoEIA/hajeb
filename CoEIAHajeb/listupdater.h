#ifndef LISTUPDATER_H
#define LISTUPDATER_H

#include <QObject>
#include <QMap>

#include "xmlconstants.h"

class NetworkManager;

namespace WebsiteLocker {

class ListUpdater : public QObject
{
    Q_OBJECT
public:
    ListUpdater(ListType listType,QObject* parent);
    void update();
public slots:
    void abortDownload();
    void finish(DownloadOperation);
signals:
    void abort();
    void finished(UpdateResult result);
private:
    bool checkForUpdate();
    void downloadServerHashFile();
    bool compareHashes();
    void removeServerHashFile();
    void renameNewHash();
    void downloadList();
    void removeOldFiles();
    void renameNewFiles();
    QString fileContent(const QString fileName);
    ListType m_listType;
    NetworkManager* m_networkManager;
    QMap<ListType, QString> m_xmlFileMap;
    QMap<ListType, QString> m_md5FileMap;
    static const QString SERVER_FILE_PREFIX;
};

}

#endif // LISTUPDATER_H
