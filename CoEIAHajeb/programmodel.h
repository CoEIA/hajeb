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
#ifndef PROGRAMMODEL_H
#define PROGRAMMODEL_H

#include <QStandardItemModel>
#include <QMultiMap>
#include "registryentry.h"

namespace ProgramLocker {

class ProgramModel : public QStandardItemModel
{
    Q_OBJECT
public:
    ProgramModel(QObject* parent=0);
    void buildItem(const QList<RegistryEntry> &list);
    void clearCacheList();
    QStringList programList();
signals:
    void itemReaded(int i);
    void exeFounded(QString fileName);
private slots:
    void cacheItem(QStandardItem*);
private:
    class BuilderItem {
    public:
        QStringList exeFiles(const QString& rootDir);
    };
    BuilderItem m_builder;
    QMultiMap<QString,QStandardItem*> m_cacheList;
};

}

#endif // PROGRAMMODEL_H
