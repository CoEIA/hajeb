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
#include "listimporter.h"
#include "xmllistreader.h"
#include <QStandardItem>
#include "listtable.h"
#include "item.h"

#include <QDebug>

namespace WebsiteLocker {

ListImporter::ListImporter(DB::ListTable* listTable, ListType listCategory) :
    m_listCategory(listCategory),m_rootItem(NULL)
{
    m_xmlReader = new XmlListReader(new QStandardItem, listCategory);
    m_dbWriter = listTable;

}

ListImporter::~ListImporter()
{
    delete m_xmlReader;
    clear();
}

void ListImporter::clear() {

    if (m_rootItem == NULL)
        return ;

    // clear all rootItem
    qDebug() << "------------------";
    qDebug() << "CLEAR ALL ITEM";
    for (int i=0; i<m_rootItem->rowCount(); ++i) {
        CategoryItem* catItem = dynamic_cast<CategoryItem*>(m_rootItem->takeChild(i));
        qDebug() << catItem->categoryName();

        if (catItem->hasChildren()) {
            for (int j=0; j<catItem->rowCount(); ++j) {
                WebItem* webItem = dynamic_cast<WebItem*>(catItem->takeChild(j));
                qDebug() << "DELETE " << webItem->url();
                delete webItem;
            }
        }

        qDebug() << "DELETE " << catItem->categoryName();
        delete catItem;
    }

    delete m_rootItem;
    m_rootItem = 0;
}

QStandardItem* ListImporter::import() {
    clear();
    m_rootItem = m_xmlReader->parse();

    // Check for NULL ptr


    return m_rootItem;
}

void ListImporter::resetDB() {
    m_dbWriter->reset(m_rootItem);
}

void ListImporter::resetAllDB() {
    m_dbWriter->resetAll(m_rootItem);
}

void ListImporter::updateDB() {
    m_dbWriter->update(m_rootItem);
}

void ListImporter::updateAllDB() {
    m_dbWriter->updateAll(m_rootItem);
}

void ListImporter::printList() {
    for (int i=0; i<m_rootItem->rowCount(); ++i) {
        CategoryItem* catItem = dynamic_cast<CategoryItem*>(m_rootItem->child(i));
        qDebug() << catItem->categoryName();

        if (catItem->hasChildren()) {
            for (int j=0; j<catItem->rowCount(); ++j) {
                WebItem* webItem = dynamic_cast<WebItem*>(catItem->child(j));
                qDebug() << webItem->url();
            }
        }
    }
}

}
