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
