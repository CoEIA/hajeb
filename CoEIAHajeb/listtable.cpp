#include "listtable.h"

#include <QtSql>
#include <QDebug>
#include "item.h"
#include "actionqueue.h"
#include "../Global/global.h"

using namespace WebsiteLocker;

namespace DB {

ListTable::Dispatcher::Dispatcher(ListTable *p) :
    parent(p)
{}

//! \todo Implement webTable and categoryTable in a polymorphic way.

void ListTable::Dispatcher::dispatch(const ActionQueue *actionQueue) {
    qDebug() << "Dispatcher called.";

    const QList<Action*>& actionList = actionQueue->actionQueue();

    QList<Action*>::const_iterator itor;
    for (itor=actionList.constBegin(); itor!=actionList.constEnd(); ++itor) {
        Action* action = *itor;
        switch (action->actionType()) {
        case Action::INSERT_ACTION:
            this->table(action)->insert(action->item());
            break;
        case Action::UPDATE_ACTION:
            this->table(action)->update(action->item());
            break;
        case Action::REMOVE_ACTION:
            this->table(action)->remove(action->item()->id());
            break;
        }
    }
}

ListTable::Table* ListTable::Dispatcher::table(Action* action) {
    if (action->item()->itemType() == Item::WEB_ITEM)
        return parent->m_webTable;
    else if (action->item()->itemType() == Item::CAT_ITEM)
        return parent->m_categoryTable;
}

ListTable::Writer::Writer(ListTable *p):
    parent(p)
{}


void ListTable::Writer::update(QStandardItem* rootItem) {
    qDebug() << "UPDATE DB CALLED";

    if (rootItem == NULL)
        return ;

    // Update categories and websites.
    for (int i=0; i<rootItem->rowCount(); ++i) {
        CategoryItem* catItem = dynamic_cast<CategoryItem*>(rootItem->child(i));
        qDebug() << catItem->categoryName();

        int catId ;
        bool newCategory = false;

        if (!parent->m_categoryTable->isAvailable(catItem->categoryName())) {
            qDebug() << "NEW CAT";
            // Insert new category and get the Id
            catId = parent->m_categoryTable->createCategory(catItem);
            newCategory = true;
        } else {
            qDebug() << "OLD CAT";
            // Get category Id
            catId = parent->m_categoryTable->categoryId(catItem->categoryName());
        }

        if (catItem->hasChildren()) {
            for (int j=0; j<catItem->rowCount(); ++j) {
                WebItem* webItem = dynamic_cast<WebItem*>(catItem->child(j));
                qDebug() << webItem->url();
                webItem->setCategoryId(catId);

                if (newCategory || !parent->m_webTable->isAvailable(webItem->host(),catId)) {
                    // insert new website.
                    parent->m_webTable->createWebSite(webItem);
                }
            }
        }
    }
}

void ListTable::Writer::updateAll(QStandardItem* rootItem) {

}

void ListTable::Writer::reset(QStandardItem* rootItem) {
    qDebug() << "RESET DB CALLED";

    if (rootItem == NULL) {
        qDebug() << "NO ITEM";
        return ;
    }

    // remove category and website for userId and list type.
    parent->m_categoryTable->removeUserCategories();

    // Insert new categories
    for (int i=0; i<rootItem->rowCount(); ++i) {
        CategoryItem* catItem = dynamic_cast<CategoryItem*>(rootItem->child(i));
        qDebug() << catItem->categoryName();
        int catId = parent->m_categoryTable->createCategory(catItem);

        if (catItem->hasChildren()) {
            for (int j=0; j<catItem->rowCount(); ++j) {
                WebItem* webItem = dynamic_cast<WebItem*>(catItem->child(j));
                qDebug() << webItem->url();
                webItem->setCategoryId(catId);
                parent->m_webTable->createWebSite(webItem);
            }
        }
    }
}

void ListTable::Writer::resetAll(QStandardItem* rootItem) {

}

ListTable::Table::Table(ListTable* p) :
    parent(p)
{}


ListTable::CategoryTable::CategoryTable(ListTable* p) :
    Table(p)
{}

int ListTable::CategoryTable::categoryCount() {
    return categoryIdList().count();
}

int ListTable::CategoryTable::maxCategoryId() const {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.exec("SELECT max(catId) FROM CategoriesTable");
    if (query.next())
        return query.value(0).toInt();
    else
        return 0;
}

CategoryItem* ListTable::CategoryTable::category(int catId) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT * from CategoriesTable WHERE catId=?");
    query.addBindValue(catId);
    query.exec();
    query.next();

    CategoryItem* item = new CategoryItem(query.value(CategoryItem::CAT_ID).toInt(),
                                          query.value(CategoryItem::CAT_NAME).toString(),
                                          query.value(CategoryItem::CAT_USER_ID).toInt(),
                                          (Qt::CheckState)query.value(CategoryItem::CAT_STATE).toInt());
    return item;
}

QList<CategoryItem *> ListTable::CategoryTable::categoryList() {
    QList<CategoryItem*> list;
    CategoryItem* item;

    foreach (int catId, categoryIdList()) {
        item = category(catId);
        list << item ;
    }

    return list;
}

QList<int> ListTable::CategoryTable::categoryIdList() {
    QList<int> idList;
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT catId from CategoriesTable WHERE userId=? AND type=?");
    query.addBindValue(parent->m_userId);
    query.addBindValue(parent->m_type);
    query.exec();
    while (query.next()) {
        idList << query.value(CategoryItem::CAT_ID).toInt();
    }

    return idList;
}

QString ListTable::CategoryTable::categoryName(int catId) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT catName from CategoriesTable WHERE catId=?");
    query.addBindValue(catId);
    query.exec();
    query.next();
    return query.value(0).toString();
}

void ListTable::CategoryTable::insert(const Item* item) {
    const CategoryItem* catItem = dynamic_cast<const CategoryItem*>(item);
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("INSERT INTO CategoriesTable(catId,catName,userId,state,type) VALUES (?,?,?,?,?);");
    query.addBindValue(catItem->id());
    query.addBindValue(catItem->categoryName());
    query.addBindValue(parent->m_userId);
    query.addBindValue(catItem->checkState());
    query.addBindValue(parent->m_type);
    query.exec();
}

void ListTable::CategoryTable::remove(int catId) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("DELETE FROM CategoriesTable WHERE catId = ?");
    query.addBindValue(catId);
    query.exec();
}

void ListTable::CategoryTable::update(const Item* item) {
    const CategoryItem* catItem = dynamic_cast<const CategoryItem*>(item);
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("UPDATE CategoriesTable SET catName=?,state=? WHERE catId=?");
    query.addBindValue(catItem->categoryName());
    query.addBindValue(catItem->checkState());
    query.addBindValue(catItem->id());
    query.exec();
}

void ListTable::CategoryTable::removeUserCategories() {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("DELETE FROM CategoriesTable WHERE userId = ? and type = ?");
    query.addBindValue(parent->m_userId);
    query.addBindValue(parent->m_type);
    query.exec();
}

int ListTable::CategoryTable::createCategory(CategoryItem *catItem) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("INSERT INTO CategoriesTable(catName,userId,state,type) VALUES (?,?,?,?);");
    query.addBindValue(catItem->categoryName());
    query.addBindValue(parent->m_userId);
    query.addBindValue(catItem->checkState());
    query.addBindValue(parent->m_type);
    query.exec();

    return parent->maxCategoryId();
}

bool ListTable::CategoryTable::isAvailable(const QString catName) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT * from CategoriesTable WHERE catName=? AND userId=? AND type=?");
    query.addBindValue(catName);
    query.addBindValue(parent->m_userId);
    query.addBindValue(parent->m_type);
    query.exec();
    if (query.next())
        return true;
    else
        return false;
}

int ListTable::CategoryTable::categoryId(const QString catName) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT catId from CategoriesTable WHERE catName=? AND userId=? AND type=?");
    query.addBindValue(catName);
    query.addBindValue(parent->m_userId);
    query.addBindValue(parent->m_type);
    query.exec();
    if (query.next())
        return query.value(0).toInt();

    Q_ASSERT(true);
}

ListTable::WebTable::WebTable(ListTable *p) :
    Table(p)
{}

int ListTable::WebTable::maxWebId() const {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.exec("SELECT max(no) FROM WebsitesTable");
    if (query.next())
        return query.value(0).toInt();
    else
        return 0;
}

QList<WebItem*> ListTable::WebTable::websiteList(int catId) {
    QList<WebItem*> list;
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT WebsitesTable.no,WebsitesTable.url,WebsitesTable.host,WebsitesTable.catId,WebsitesTable.state from WebsitesTable,CategoriesTable WHERE WebsitesTable.catId = CategoriesTable.catId AND CategoriesTable.catId=?");
    query.addBindValue(catId);
    query.exec();
    while (query.next()) {
        WebItem* item = new WebItem(query.value(WebItem::WEB_NO).toInt(),
                                    query.value(WebItem::WEB_URL).toString(),
                                    query.value(WebItem::WEB_HOST).toString(),
                                    query.value(WebItem::WEB_CAT_ID).toInt(),
                                    (Qt::CheckState)query.value(WebItem::WEB_STATE).toInt());
        list << item;
    }

    return list;
}

void ListTable::WebTable::insert(const Item* item) {
    const WebItem* webItem = dynamic_cast<const WebItem*>(item);
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("INSERT INTO WebsitesTable(no,url,host,catId,state) VALUES (?,?,?,?,?);");
    query.addBindValue(webItem->id());
    query.addBindValue(webItem->url());
    query.addBindValue(webItem->host());
    query.addBindValue(webItem->categoryId());
    query.addBindValue(webItem->checkState());
    query.exec();
}

void ListTable::WebTable::remove(int webNo) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("DELETE FROM WebsitesTable WHERE no = ?");
    query.addBindValue(webNo);
    query.exec();
}

void ListTable::WebTable::update(const Item* item) {
    const WebItem* webItem = dynamic_cast<const WebItem*>(item);
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("UPDATE WebsitesTable SET url=?,host=?,state=? WHERE no=?");
    query.addBindValue(webItem->url());
    query.addBindValue(webItem->host());
    query.addBindValue(webItem->checkState());
    query.addBindValue(webItem->id());
    query.exec();
}

void ListTable::WebTable::createWebSite(WebItem *webItem) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("INSERT INTO WebsitesTable(url,host,catId,state) VALUES (?,?,?,?);");
    query.addBindValue(webItem->url());
    query.addBindValue(webItem->host());
    query.addBindValue(webItem->categoryId());
    query.addBindValue(webItem->checkState());
    query.exec();
}

bool ListTable::WebTable::isAvailable(const QString webHost, int catId) {
    QSqlQuery query(QSqlDatabase::database(PCSDB_NAME));
    query.prepare("SELECT * from WebsitesTable WHERE host=? AND catId=?");
    query.addBindValue(webHost);
    query.addBindValue(catId);
    query.exec();
    if (query.next())
        return true;
    else
        return false;
}

void ListTable::setListType(ListType type) {
    m_type = type;
}

ListTable::ListType ListTable::listType() const {
    return m_type;
}

ListTable::ListType ListTable::readUserListType(int userId)  {
    QSqlQuery query(QSqlDatabase::database(LSPDB_NAME));
    query.prepare("SELECT listType from UsersTable WHERE userId=?");
    query.addBindValue(userId);
    query.exec();
    if (query.next()) {
        ListType type = (ListType)query.value(0).toInt();
        this->setListType(type);
        return type;
    }

    Q_ASSERT(true); // Force error
}

void ListTable::writeUserListType(DB::ListTable::ListType type) {
    QSqlQuery query(QSqlDatabase::database(LSPDB_NAME));
    query.prepare("UPDATE UsersTable SET listType=? WHERE userId=?");
    query.addBindValue(type);
    query.addBindValue(m_userId);
    query.exec();

    setListType(type);
}

void ListTable::setUserId(int userId) {
    m_userId = userId;
}

int ListTable::userId() const {
    return m_userId;
}

void ListTable::apply(const ActionQueue* actionQueue) {
    m_dispatcher->dispatch(actionQueue);
}

void ListTable::save() {

}

void ListTable::update(QStandardItem* rootItem) {
    m_writer->update(rootItem);
}

void ListTable::updateAll(QStandardItem* rootItem) {
    m_writer->updateAll(rootItem);
}

void ListTable::reset(QStandardItem* rootItem) {
    m_writer->reset(rootItem);
}

void ListTable::resetAll(QStandardItem* rootItem) {
    m_writer->resetAll(rootItem);
}

void ListTable::exportToLSPDb() {
    // remove website for userId in LSPDB
    QSqlQuery query(QSqlDatabase::database(LSPDB_NAME));
    query.prepare("DELETE FROM LockedWebsitesTable WHERE userId=?");
    query.addBindValue(m_userId);
    query.exec();

    // read all category id for userId and listtype
    foreach (int catId, m_categoryTable->categoryIdList()) {
        // read all related website
        foreach (WebItem* webItem, m_webTable->websiteList(catId)) {
            // Add these new websites to LSPDB
            if (webItem->checkState() == Qt::Checked) {
                query.prepare("INSERT INTO LockedWebsitesTable(userId,url,host,lockDate) VALUES (?,?,?,?);");
                query.addBindValue(m_userId);
                query.addBindValue(webItem->url());
                query.addBindValue(webItem->host());
                query.addBindValue(QDate::currentDate());
                if (!query.exec())
                    qDebug() << "Unable to insert website in the locked table";
            }

            delete webItem;
        }
    }
}

}




