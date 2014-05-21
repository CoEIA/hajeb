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
