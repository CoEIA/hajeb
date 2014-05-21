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
