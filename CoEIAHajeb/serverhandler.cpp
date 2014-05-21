#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include "serverhandler.h"
#include "constant.h"
#include "../Global/global.h"

ServerHandler::ServerHandler(QObject *parent):
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

QString ServerHandler::verifyKey(QString key) {
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(URL+"software/hajeb/keys.php?key=" + key)));

    QEventLoop eventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return result;
}

QString ServerHandler::getVersionNumberFile() {
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(URL+"software/hajeb/bin/version.txt")));

    QEventLoop eventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return result;
}

QString ServerHandler::registerKeyOnline(QString key, QString ip) {
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    QString url = QString(URL+"software/hajeb/add.php?serial=%1&ip=%2").arg(key,ip);
    manager->get(QNetworkRequest(QUrl(url)));

    QEventLoop eventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    return result;
}

void ServerHandler::replyFinished(QNetworkReply *np) {
    if (np->error() != QNetworkReply::NoError) {
        result = "ERORR";
        return;
    }

    QByteArray data = np->readAll();
    QString str(data);
    result = str;
}
