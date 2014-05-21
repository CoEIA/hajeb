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
#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;

class ServerHandler : public QObject
{
  Q_OBJECT
public:
    ServerHandler(QObject* parent=0);
    QString verifyKey(QString key);
    QString registerKeyOnline(QString key,QString ip);
    QString getVersionNumberFile();
    //QByteArray getUpdater();

private slots:
    void replyFinished(QNetworkReply *np);
private:
    QNetworkAccessManager* manager;
    QString result;
    QByteArray resultBytes;
    bool info;
};

#endif // SERVERHANDLER_H
