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
