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
#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>
#include "listtable.h"
#include "actionqueue.h"
#include "action.h"

class QStandardItem;
class Item;
class CategoryItem;
class WebItem;

namespace WebsiteLocker {


class TreeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    TreeModel(DB::ListTable::ListType type);
    TreeModel(int userId, DB::ListTable::ListType type);
    ~TreeModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
public slots:
    void addCategory(QString name);
    void addWebsite(QString url, QStandardItem* parent);
    void addItem(QStandardItem* item,QStandardItem* parent);
    void appendItem(Item* item, Item* parent);
    void removeItem(QStandardItem* item);
    void removeItem(QModelIndex index);
    void import(int userId);
    bool save();
    void updateAction(QStandardItem* item);
    void updateAction(Item* item, Action::ActionType type);
    void setListType(DB::ListTable::ListType type);
    DB::ListTable::ListType readUserListType(int userId);
    void writeUserListType(DB::ListTable::ListType type);
    DB::ListTable::ListType listType() const;
    DB::ListTable* database() const;
    bool hasChanged();
    void clearActions();
    void exportToLSPDb();
private:

    void applyRole(Item* item);
    DB::ListTable* m_database;
    ActionQueue* m_actionQueue;
    int m_categoryCounter;
    int m_webCounter;
};

}

#endif // TREEMODEL_H
