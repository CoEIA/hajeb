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
#ifndef REGISTRYREADER_H
#define REGISTRYREADER_H

#include <QSettings>
#include <QMap>
#include <QList>
#include <QStringList>

#include "registryentry.h"

namespace ProgramLocker {

class RegistryReader
{
public:
    RegistryReader();
    ~RegistryReader();
    QList<RegistryEntry> read(const QString& path);
private:
    QList<RegistryEntry> readUninstallEntries();
    QStringList readChildGroup();
    RegistryEntry readChildKeys(const QString& entry);
    bool filter(const QString& key);
    QSettings* m_settings;
    QString m_path;
};

}

#endif // REGISTRYREADER_H
