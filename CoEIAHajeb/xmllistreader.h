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
#ifndef XMLLISTREADER_H
#define XMLLISTREADER_H

#include <QXmlStreamReader>
#include <QMap>
#include <QList>
#include "listimporter.h"

class QStandardItem;

#include "websitelocker.h"
using namespace WebsiteLocker;

class XmlListReader
{
public:
    XmlListReader(QStandardItem* rootItem, ListImporter::ListType listCategory);
    ~XmlListReader();
    QStandardItem* parse();
private:
    Item* readCategory();
    QList<QStandardItem*> readWebList();
    ListImporter::ListType m_listCategory;
    QStandardItem* m_rootItem;
    QXmlStreamReader m_reader;
    QMap<ListImporter::ListType, QString> m_table;
};

#endif // XMLLISTREADER_H
