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
