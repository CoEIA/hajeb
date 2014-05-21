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
