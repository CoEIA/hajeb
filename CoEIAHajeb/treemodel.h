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
