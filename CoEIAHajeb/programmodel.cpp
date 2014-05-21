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
#include <QStandardItem>
#include <QDir>
#include <QLabel>
#include <QFileInfo>
#include <QDebug>
#include <QApplication>

#include "programmodel.h"

namespace ProgramLocker {

ProgramModel::ProgramModel(QObject *parent) :
    QStandardItemModel(parent)
{
    connect(this,SIGNAL(itemChanged(QStandardItem*)),SLOT(cacheItem(QStandardItem*)));
}

void ProgramModel::buildItem(const QList<RegistryEntry> &list) {
    int i=0;
    foreach (RegistryEntry entry, list) {
        QApplication::processEvents();
        QStandardItem* item = new QStandardItem(entry.path());
        item->setIcon(QIcon(":/images/category.png"));
        item->setForeground(QColor(Qt::darkBlue));

        this->invisibleRootItem()->appendRow(item);

        emit itemReaded(++i);
        emit exeFounded(entry.entryName());

        // find all exe files in this directory
        QStringList fileList = m_builder.exeFiles(entry.path());
        foreach (QString file, fileList) {
            QStandardItem* childItem = new QStandardItem(file);
            childItem->setCheckable(true);
            childItem->setIcon(QIcon(":/images/exec.png")); //! Improvement: Extract exe icon.
            item->appendRow(childItem);
            QApplication::processEvents();
        }
    }
}


QStringList ProgramModel::BuilderItem::exeFiles(const QString &rootDir) {
    QStringList exefiles;

    QApplication::processEvents();

    QDir dir(rootDir);

    QFileInfoList dirs = dir.entryInfoList(QDir::AllDirs|QDir::NoDotAndDotDot);
    foreach (QFileInfo file, dirs) {
        exefiles +=  exeFiles(file.absoluteFilePath());
    }

    dir.setNameFilters(QStringList() << "*.exe");
    exefiles += dir.entryList();

    return exefiles;
}

void ProgramModel::cacheItem(QStandardItem* item) {
    if (item->checkState() == Qt::Checked) {
        qDebug() << "CHECK " << item->text();
//        m_cacheList.append(item->text());
        m_cacheList.insert(item->text(),item);
    } else if (item->checkState() == Qt::Unchecked) {
        qDebug() << "UNCHECK " << item->text();
//        m_cacheList.removeAll(item->text());
        m_cacheList.remove(item->text(),item);
    }
}

void ProgramModel::clearCacheList() {
    QMapIterator<QString, QStandardItem*> i(m_cacheList);
    while (i.hasNext()) {
        i.next();
        i.value()->setCheckState(Qt::Unchecked);
    }

    m_cacheList.clear();
}

QStringList ProgramModel::programList() {
    return m_cacheList.keys();
}

}

