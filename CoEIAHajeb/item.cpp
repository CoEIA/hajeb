#include "item.h"

namespace WebsiteLocker {
Item::Item(int id, const QString& name, ItemType itemType, Qt::CheckState state) :
    QStandardItem(name), m_id(id), m_itemType(itemType)
{
    this->setCheckable(true);
    this->setCheckState(state);
}

Item::~Item()
{}


CategoryItem::CategoryItem(int catId, const QString& name, int userId, Qt::CheckState state) :
    Item(catId,name,Item::CAT_ITEM,state), m_userId(userId)
{}

CategoryItem::~CategoryItem()
{}

WebItem::WebItem(int webNo, const QString& url, const QString& host, int catId, Qt::CheckState state) :
    Item(webNo,url,Item::WEB_ITEM,state), m_host(host), m_catId(catId)
{}


WebItem::~WebItem()
{}


DataItem::DataItem(int id, ItemType itemType) :
    Item(id,QString(),itemType)
{}

DataItem::~DataItem()
{}


}
