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
