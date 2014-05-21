#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include "networkmanager.h"
#include "../Global/global.h"

NetworkManager::NetworkManager() :
    m_manager(new QNetworkAccessManager(this)),m_reply(0)
{
    connect(m_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
}

NetworkManager::~NetworkManager() {
}

void NetworkManager::replyFinished(QNetworkReply* reply) {
    QUrl url = reply->url();
    if (reply->error()) {
        qDebug() << "Download of: " << url.toString() << "Failed: " << reply->errorString();
        return ;
    }

    QString filePath = url.path();
    QString fileName = QFileInfo(filePath).fileName();

    if (fileName.isEmpty())
        fileName = "download";

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        file.write(reply->readAll());
        file.close();
    }

    qDebug() << "Download of: " << url.toString() << "Success";

    // Get reply operation type
    DownloadOperation op = m_replyMap[m_reply];
    m_replyMap.remove(m_reply);

    // remove reply
    reply->deleteLater();
    m_reply = 0;

    emit finished(op);
}

void NetworkManager::getFile(QString fileName, DownloadOperation operation) {
    fileName.prepend(URL + "software/hajeb/list/");
    m_reply = m_manager->get(QNetworkRequest(QUrl(fileName)));
    m_replyMap[m_reply] = operation;
}

void NetworkManager::abort() {
    if (!m_reply)
        return ;

    m_reply->abort();
    m_reply->deleteLater();
    m_reply = 0;
}
