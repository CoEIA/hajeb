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
