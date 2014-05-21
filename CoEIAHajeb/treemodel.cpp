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
#include "treemodel.h"

#include <QDebug>
#include "item.h"
#include "listtable.h"
#include "action.h"
#include "actionqueue.h"

namespace WebsiteLocker {

TreeModel::TreeModel(DB::ListTable::ListType type) :
     QStandardItemModel(), m_database(new DB::ListTable(-1,type)), m_actionQueue(new ActionQueue)
{
    m_categoryCounter = m_webCounter = 0;
    connect(this,SIGNAL(itemChanged(QStandardItem*)),SLOT(updateAction(QStandardItem*)));

    setHorizontalHeaderLabels(QStringList() << "one");
}

TreeModel::TreeModel(int userId, DB::ListTable::ListType type) :
    QStandardItemModel(), m_database(new DB::ListTable(userId,type)), m_actionQueue(new ActionQueue)
{
    m_categoryCounter = m_webCounter = 0;
    import(userId);
    connect(this,SIGNAL(itemChanged(QStandardItem*)),SLOT(updateAction(QStandardItem*)));
}

TreeModel::~TreeModel()
{
    delete m_database;
    delete m_actionQueue;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DecorationRole) {
        if (!index.parent().isValid()) {
            if (itemFromIndex(index)->hasChildren())
                return QIcon(":/images/addCat.png");
            else
                return QIcon(":/images/deleteItem.png");
        } else
            return QIcon(":/images/addWeb.png");
    } else if (role == Qt::FontRole && itemFromIndex(index)->checkState() == Qt::Unchecked) {
        QFont f = itemFromIndex(index)->font();
        f.setStrikeOut(true);
        return f;
    } else if (role == Qt::FontRole && index.parent().isValid()) {
        QFont f = itemFromIndex(index)->font();
        f.setUnderline(true);
        return f;
    } else if (role == Qt::ForegroundRole && itemFromIndex(index)->checkState() == Qt::Unchecked) {
        return Qt::gray;
    } else if (role == Qt::ForegroundRole && index.parent().isValid()) {
        return Qt::blue;
    } /*else if (role == Qt::DisplayRole && !index.parent().isValid()) {
        int total = itemFromIndex(index)->rowCount();
        int checked = 0;
        for (int i=0; i<total; ++i) {
            if (itemFromIndex(index)->child(i)->checkState() == Qt::Checked)
                checked ++;
        }

        QVariant v = QString(" (%1/%2)").arg(checked).arg(total);
        return QStandardItemModel::data(index,role).toString() + v.toString();

    }*/ else
        return QStandardItemModel::data(index,role);
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    QStandardItem *item = this->itemFromIndex(index);

    if (role == Qt::CheckStateRole) {
        for (int i = 0; i<item->rowCount(); i++)
            setData(item->child(i)->index(), value, role);
    }

    return QStandardItemModel::setData(index, value, role);
}

void TreeModel::import(int userId) {
    qDebug() << "Import called for user: " << userId;

    // clear the tree model
    this->clear();

    // clear the model actions queue
    clearActions();

    // set user id
    m_database->setUserId(userId);

    // fetch data and counter
    m_categoryCounter = m_database->maxCategoryId();
    m_webCounter = m_database->maxWebId();


    foreach (CategoryItem* item, m_database->categoryList()) {
        // Add category to the tree model
        appendItem(item,static_cast<Item*>(this->invisibleRootItem()));

        // Get the child of the current category
        QList<WebItem*> websites = m_database->websiteList(item->categoryId());

        // Add child to the tree model
        foreach (WebItem* webItem, websites) {
            appendItem(webItem,item);
        }
    }
}

bool TreeModel::save() {
    // save action queue
    m_database->apply(m_actionQueue);

    // clear actions queue
    clearActions();
}

void TreeModel::appendItem(Item* item, Item* parent) {
    // Append item within parent
    parent->appendRow(item);

    // Apply checkbox role to the item
    applyRole(item);
}

void TreeModel::addItem(QStandardItem* standardItem, QStandardItem* parent) {
    Item* item = dynamic_cast<Item*>(standardItem);

    parent->appendRow(item);

    // Add INSERT action.
    updateAction(item,Action::INSERT_ACTION);
}

void TreeModel::removeItem(QStandardItem* standardItem) {
    Item* item = dynamic_cast<Item*>(standardItem);

    // Add the action. if it is parent add action for the child too
    if (item->hasChildren()) {
        for (int i=0; i<item->rowCount(); ++i)
            updateAction(static_cast<Item*>(item->child(i)),Action::REMOVE_ACTION);
    }

    updateAction(item,Action::REMOVE_ACTION);

    // Remove the item.
    this->removeRow(item->index().row(),item->index().parent());
}

void TreeModel::removeItem(QModelIndex index) {
//    this->removeRow(index.row(),index.parent());
}

void TreeModel::applyRole(Item* item) {
    // Check parent checkbox
    if (item->parent()) {
        if (item->parent()->checkState() == Qt::Unchecked)
            item->setCheckState(Qt::Unchecked);
    }
}

void TreeModel::addCategory(QString name) {
    addItem(new CategoryItem(++m_categoryCounter,name,m_database->userId()),this->invisibleRootItem());
}


void TreeModel::addWebsite(QString url, QStandardItem* parent) {
    CategoryItem* item = dynamic_cast<CategoryItem*>(parent);
    if (!item)
        return;

    QString host = url.remove("http://");
    host = host.remove("www.");

    addItem(new WebItem(++m_webCounter,url,host,item->id()),parent);
}


void TreeModel::updateAction(QStandardItem *item) {
    Action* action = ActionFactory::createAction(dynamic_cast<Item*>(item), Action::UPDATE_ACTION);
    m_actionQueue->addAction(action);
}

void TreeModel::updateAction(Item* item, Action::ActionType type) {
    Action* action = ActionFactory::createAction(item, type);
    m_actionQueue->addAction(action);
}

DB::ListTable * TreeModel::database() const {
    return m_database;
}

void TreeModel::setListType(DB::ListTable::ListType type) {
    m_database->setListType(type);
}

DB::ListTable::ListType TreeModel::listType() const {
    return m_database->listType();
}

DB::ListTable::ListType TreeModel::readUserListType(int userId) {
    return m_database->readUserListType(userId);
}

void TreeModel::writeUserListType(DB::ListTable::ListType type) {
    m_database->writeUserListType(type);
}

bool TreeModel::hasChanged() {
    if (m_actionQueue->actionQueue().count())
        return true;
    else
        return false;
}

void TreeModel::clearActions() {
    m_actionQueue->clear();
}

void TreeModel::exportToLSPDb() {
    m_database->exportToLSPDb();
}

}

