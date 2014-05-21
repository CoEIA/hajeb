#ifndef ITEM_H
#define ITEM_H

#include <QStandardItem>
#include <QDebug>

namespace WebsiteLocker {
/*!
 *  \brief Abstract item.
 *
 *  Abstract Item will be used in the model and seen by view.
 */

class Item : public QStandardItem
{
public:
    enum ItemType {
        CAT_ITEM,
        WEB_ITEM
    };

    Item(int id, const QString& name, ItemType itemType, Qt::CheckState state = Qt::Checked );
    virtual ~Item() = 0;

    void setItemState(Qt::CheckState state) { this->setCheckState(state); }
    Qt::CheckState itemState() const { return this->checkState(); }

    void setItemType(ItemType itemType) { m_itemType = itemType; }
    ItemType itemType() const { return m_itemType; }

    void setId(int id) { m_id = id; }
    int id() const { return m_id; }

    void printDebug(QString spacer = "") const {
        qDebug() << spacer << "------------------------";
        qDebug() << spacer << "Item name: "   << this->text();
        qDebug() << spacer << "Item state: "  << this->itemState();
    }
protected:
    int m_id;
    ItemType m_itemType;
};


class CategoryItem : public Item
{
public:
    enum Field {
        CAT_ID,
        CAT_NAME,
        CAT_USER_ID,
        CAT_STATE
    };

    CategoryItem(int catId, const QString& name, int userId, Qt::CheckState state = Qt::Checked);
    ~CategoryItem();

    void setCategoryId(int catId) { m_id = catId; }
    int categoryId() const { return m_id; }

    void setUserId(int userId) { m_userId = userId; }
    int userId() const { return m_userId; }

    QString categoryName() const { return this->text(); }
    void setCategoryName(const QString& name) { this->setText(name); }

    void printDebug() const {
        Item::printDebug();
        qDebug() << "Category Id: " << this->categoryId();
        qDebug() << "User Id: " << this->userId();
    }

private:
    int m_userId;
};

class WebItem : public Item
{
public:
    enum Field {
        WEB_NO,
        WEB_URL,
        WEB_HOST,
        WEB_CAT_ID,
        WEB_STATE
    };

    WebItem(int webNo, const QString& url, const QString& host, int catId, Qt::CheckState state = Qt::Checked);
    ~WebItem();

    void setUrl(const QString& url) { this->setText(url); }
    QString url() const { return this->text(); }

    void setHost(QString host) { m_host = host; }
    QString host() const { return m_host; }

    void setCategoryId(int catId) { m_catId = catId; }
    int categoryId() const { return m_catId; }

    void setWebNo(int webNo) { m_id = webNo; }
    int webNo() const { return m_id; }

    void printDebug() const {
        Item::printDebug("\t");
        qDebug() << "\t" << "Host name: "   << this->host();
        qDebug() << "\t" << "Category Id: " << this->categoryId();
    }

private:
    QString m_host;
    int m_catId;
};


class DataItem : public Item
{
public:
    DataItem(int id, ItemType itemType);
    ~DataItem();
};

}

#endif // ITEM_H
