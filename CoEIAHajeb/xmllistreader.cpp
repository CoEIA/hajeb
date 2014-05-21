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
#include <QDebug>

#include "xmllistreader.h"
#include "xmlconstants.h"
#include "item.h"

XmlListReader::XmlListReader(QStandardItem* rootItem, ListImporter::ListType listCategory) :
    m_listCategory(listCategory),m_rootItem(rootItem)
{
    m_table[ListImporter::WHITE_LIST] = "wlist.xml";
    m_table[ListImporter::BLACK_LIST] = "blist.xml";
}

XmlListReader::~XmlListReader()
{
}

QStandardItem* XmlListReader::parse() {
    QFile file(m_table[m_listCategory]);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open the file";
        return NULL;
    }

    qDebug() << "XML FILE TO IMPORT: " << m_table[m_listCategory];

    m_reader.setDevice(&file);

    // Start parsing the list.
    while (!m_reader.atEnd() && !m_reader.hasError()) {
        QXmlStreamReader::TokenType token = m_reader.readNext();

        if (token == QXmlStreamReader::StartDocument)
            continue;

        if (token == QXmlStreamReader::StartElement) {
            if (m_reader.name() == XML_TAG_LIST)
                continue;

            m_rootItem->appendRow(readCategory());
        }
    }

    if (m_reader.hasError()) {
        qDebug() << "xml parse error: " << m_reader.errorString();
    }

    m_reader.clear();
    return m_rootItem;
}

Item* XmlListReader::readCategory() {
    qDebug() << "TAG NAME: " << m_reader.name().toString();

    // Read attributes
    QStringRef name = m_reader.attributes().value(XML_ATTR_NAME);
    qDebug() << "CAT NAME: " << name.toString();

    Item* item = new CategoryItem(-1,name.toString(),-1);

    // Read close tag
    m_reader.readNext();

    // read webite
    item->appendRows(readWebList());

    return item;
}

QList<QStandardItem*> XmlListReader::readWebList() {
    QList<QStandardItem*> list;

    m_reader.readNext();
    qDebug() << "WEB TAG NAME: " << m_reader.name().toString();

    while (m_reader.tokenType() != QXmlStreamReader::EndElement || m_reader.name() != XML_TAG_CAT) {
        QStringRef url = m_reader.attributes().value(XML_ATTR_URL);
        QStringRef host = m_reader.attributes().value(XML_ATTR_HOST);

        if (url.toString().isEmpty()) {
            m_reader.readNext();
            continue;
        }

        qDebug() << "URL" << url;
        qDebug() << "HOST" << host;

        list.append(new WebItem(-1,url.toString(),host.toString(),-1));
        m_reader.readNext();
    }

    return list;
}
