#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QMap>
#include "xmlconstants.h"

class QNetworkAccessManager;
class QNetworkReply;

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager();
    ~NetworkManager();
    void getFile(QString fileName,DownloadOperation operation);
private slots:
    void abort();
    void replyFinished(QNetworkReply*);
signals:
    void finished(DownloadOperation);
private:
    QNetworkAccessManager* m_manager;
    QNetworkReply* m_reply;
    QMap<QNetworkReply*,DownloadOperation> m_replyMap;
};

#endif // NETWORKMANAGER_H
