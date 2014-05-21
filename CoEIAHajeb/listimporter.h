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
#ifndef LISTIMPORTER_H
#define LISTIMPORTER_H

#include "websitelocker.h"
using namespace WebsiteLocker;

class XmlListReader;
class QStandardItem;

namespace DB {
class ListTable;
}

namespace WebsiteLocker {

class ListImporter
{
public:
    enum ListType {
        BLACK_LIST,
        WHITE_LIST
    };

    ListImporter(DB::ListTable* listTable, ListType listCategory);
    ~ListImporter();

    QStandardItem* import();
    void resetDB();
    void resetAllDB();
    void updateDB();
    void updateAllDB();

    void printList();

private:
    void clear(); // remove m_rootItem;
    ListType m_listCategory;
    QStandardItem* m_rootItem;
    XmlListReader* m_xmlReader;
    DB::ListTable* m_dbWriter;
};

}

#endif // LISTIMPORTER_H
