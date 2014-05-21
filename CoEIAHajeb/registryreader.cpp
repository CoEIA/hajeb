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
#include "registryreader.h"
#include "../Global/global.h"

namespace ProgramLocker {

RegistryReader::RegistryReader() :
    m_settings(0)
{}

RegistryReader::~RegistryReader()
{
    delete m_settings;
}

QList<RegistryEntry> RegistryReader::read(const QString &path) {
    if (m_settings)
        delete m_settings;

    m_path = path;
    m_settings = new QSettings(path,QSettings::NativeFormat);
    return readUninstallEntries();
}

QList<RegistryEntry> RegistryReader::readUninstallEntries() {
    QList<RegistryEntry> list;
    foreach (QString entry,readChildGroup()) {
        if (entry.startsWith("{"))
            continue;
        RegistryEntry registryEntry = readChildKeys(entry);
        if (    registryEntry.value(RegistryEntry::UNINSTALL_STRING) != "" &&
                !registryEntry.path().toLower().contains(".exe") &&
                registryEntry.path() != "." &&
                !registryEntry.path().toLower().contains("system32")
           )
            list << registryEntry;
    }
    return list;
}

QStringList RegistryReader::readChildGroup() {
    return m_settings->childGroups();
}

RegistryEntry RegistryReader::readChildKeys(const QString& entry) {
    QMap<QString, QString> map;

    QString groupName = QString(m_path) + "\\" + entry;
    QSettings subSettings(groupName,QSettings::NativeFormat);

    foreach (QString key, subSettings.allKeys()) {
        QString value = subSettings.value(key).toString();
        if (filter(key))
            map[key] = value;
    }

    return RegistryEntry(map,entry);
}

bool RegistryReader::filter(const QString &key) {
    if (key == RegistryEntry::UNINSTALL_STRING || key == RegistryEntry::DISPLAY_ICON || key == RegistryEntry::DISPLAY_NAME)
        return true;
    else
        return false;
}

}
