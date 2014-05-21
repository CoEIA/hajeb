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
#include <QFile>
#include <QDir>
#include "networkmanager.h"
#include "listupdater.h"

namespace WebsiteLocker {

const QString ListUpdater::SERVER_FILE_PREFIX = "SERVER_";

ListUpdater::ListUpdater(ListType listType, QObject* parent) :
    QObject(parent), m_listType(listType),m_networkManager(new NetworkManager)
{
    m_xmlFileMap[WHITE_LIST] = "wlist.xml";
    m_xmlFileMap[BLACK_LIST] = "blist.xml";

    m_md5FileMap[WHITE_LIST] = "wmd5.txt";
    m_md5FileMap[BLACK_LIST] = "bmd5.txt";

    connect(m_networkManager,SIGNAL(finished(DownloadOperation)),this,SLOT(finish(DownloadOperation)));
    connect(this,SIGNAL(abort()),m_networkManager,SLOT(abort()));
}

void ListUpdater::abortDownload() {
    emit abort();
}

void ListUpdater::finish(DownloadOperation operation) {
    if (operation == DOWNLOAD_HASH) {
        if (compareHashes()) {
            // No new updates
            removeServerHashFile();
            emit finished(NO_UPDATE);
            return ;
        }

        downloadList();

    } else if (operation == DOWNLOAD_LIST) {
        removeOldFiles();
        renameNewFiles();
        emit finished(UPDATE);
    }

}

void ListUpdater::update() {
    checkForUpdate();
}

bool ListUpdater::checkForUpdate() {
    downloadServerHashFile();
}

void ListUpdater::downloadServerHashFile() {
    // Get md5 hash from the server
    m_networkManager->getFile(QString(m_md5FileMap[m_listType]).prepend(SERVER_FILE_PREFIX),DOWNLOAD_HASH);
}

bool ListUpdater::compareHashes() {
    // compare new hash with current hash

    QString localHash = fileContent(m_md5FileMap[m_listType]);
    QString serverHash = fileContent(QString(m_md5FileMap[m_listType]).prepend(SERVER_FILE_PREFIX));

    if (localHash == serverHash)
        return true;

    return false; // HASH NOT MATCH
}

void ListUpdater::removeServerHashFile() {
    QDir dir;
    dir.remove(QString(m_md5FileMap[m_listType]).prepend(SERVER_FILE_PREFIX));
}

void ListUpdater::renameNewHash() {

}

void ListUpdater::downloadList() {
    m_networkManager->getFile(QString(m_xmlFileMap[m_listType]).prepend(SERVER_FILE_PREFIX),DOWNLOAD_LIST);
}


void ListUpdater::removeOldFiles() {
    QDir dir;
    dir.remove(QString(m_md5FileMap[m_listType]));
    dir.remove(QString(m_xmlFileMap[m_listType]));
}

void ListUpdater::renameNewFiles() {
    QDir dir;
    dir.rename(QString(m_md5FileMap[m_listType]).prepend(SERVER_FILE_PREFIX),QString(m_md5FileMap[m_listType]));
    dir.rename(QString(m_xmlFileMap[m_listType]).prepend(SERVER_FILE_PREFIX),QString(m_xmlFileMap[m_listType]));
}

QString ListUpdater::fileContent(const QString fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        return file.readAll();
    }

    return QString();
}

}
