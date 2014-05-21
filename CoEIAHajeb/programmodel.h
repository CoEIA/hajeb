#ifndef PROGRAMMODEL_H
#define PROGRAMMODEL_H

#include <QStandardItemModel>
#include <QMultiMap>
#include "registryentry.h"

namespace ProgramLocker {

class ProgramModel : public QStandardItemModel
{
    Q_OBJECT
public:
    ProgramModel(QObject* parent=0);
    void buildItem(const QList<RegistryEntry> &list);
    void clearCacheList();
    QStringList programList();
signals:
    void itemReaded(int i);
    void exeFounded(QString fileName);
private slots:
    void cacheItem(QStandardItem*);
private:
    class BuilderItem {
    public:
        QStringList exeFiles(const QString& rootDir);
    };
    BuilderItem m_builder;
    QMultiMap<QString,QStandardItem*> m_cacheList;
};

}

#endif // PROGRAMMODEL_H
