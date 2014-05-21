#ifndef LISTTABLE_H
#define LISTTABLE_H

#include <QList>
#include "websitelocker.h"

using namespace WebsiteLocker;

class QStandardItem;

namespace DB {


class ListTable
{
public:

    enum ListType {BLACK_LIST, WHITE_LIST};

    ListTable(int userId, ListType type = BLACK_LIST) :
        m_userId(userId), m_type(type)
    {
        m_categoryTable = new CategoryTable(this);
        m_webTable = new WebTable(this);
        m_dispatcher = new Dispatcher(this);
        m_writer = new Writer(this);
    }

    ~ListTable()
    {
        delete m_categoryTable;
        delete m_webTable;
        delete m_dispatcher;
        delete m_writer;
    }

    int categoryCount() { return m_categoryTable->categoryCount(); }
    int maxCategoryId() const { return m_categoryTable->maxCategoryId(); }
    CategoryItem* category(int catId) { return m_categoryTable->category(catId); }
    QList<CategoryItem*> categoryList() { return m_categoryTable->categoryList(); }

    int maxWebId() const { return m_webTable->maxWebId(); }
    QList<WebItem*> websiteList(int catId) { return m_webTable->websiteList(catId); }

    void setListType(ListType type);
    ListType listType() const;

    ListType readUserListType(int userId) ;
    void writeUserListType(DB::ListTable::ListType type);

    void setUserId(int userId);
    int userId() const;

    void apply(const ActionQueue* actionQueue);

    void save();

    void update(QStandardItem* rootItem);
    void updateAll(QStandardItem* rootItem);
    void reset(QStandardItem* rootItem);
    void resetAll(QStandardItem* rootItem);

    void exportToLSPDb();
private:


    class Table
    {
    public:
        Table(ListTable* p);
        virtual void insert(const Item* item) = 0;
        virtual void remove(int id) = 0;
        virtual void update(const Item* item) = 0;
        ListTable* parent;
    };

    class CategoryTable : public Table
    {
    public:
        CategoryTable(ListTable* p);
        int categoryCount();
        int maxCategoryId() const;
        CategoryItem* category(int catId);
        QList<CategoryItem*> categoryList();
        QList<int> categoryIdList();
        QString categoryName(int catId);
        void insert(const Item* item);
        void remove(int id);
        void update(const Item* item);
        void removeUserCategories();
        int createCategory(CategoryItem* webItem);
        bool isAvailable(const QString catName);
        int categoryId(const QString catName);
    };

    class WebTable : public Table
    {
    public:
        WebTable(ListTable* p);
        int maxWebId() const;
        QList<WebItem*> websiteList(int catId);
        void insert(const Item* item);
        void remove(int id);
        void update(const Item* item);
        void createWebSite(WebItem* webItem);
        bool isAvailable(const QString webHost, int catId);
    };

    class Dispatcher
    {
    public:
        Dispatcher(ListTable* p);
        void dispatch(const ActionQueue* actionQueue);
        Table* table(Action* action);
        ListTable* parent;
    };

    class Writer
    {
    public:
        Writer(ListTable* p);
        void update(QStandardItem* rootItem);
        void updateAll(QStandardItem* rootItem);
        void reset(QStandardItem* rootItem);
        void resetAll(QStandardItem* rootItem);
        ListTable* parent;
    };

    friend class Table;
    friend class Dispatcher;
    friend class Writer;

    int m_userId;
    ListType m_type;

    CategoryTable* m_categoryTable;
    WebTable* m_webTable;

    Dispatcher* m_dispatcher;
    Writer* m_writer;
};

}

#endif // LISTTABLE_H
