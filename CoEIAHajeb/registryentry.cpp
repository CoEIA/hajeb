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
#include <QDir>
#include <QFileInfo>
#include <QStringList>
#include <QDebug>
#include "registryentry.h"

namespace ProgramLocker {

const QString RegistryEntry::DISPLAY_NAME = "DisplayName";
const QString RegistryEntry::DISPLAY_ICON = "DisplayIcon";
const QString RegistryEntry::UNINSTALL_STRING = "UninstallString";

RegistryEntry::RegistryEntry(const QMap<QString, QString> &object, const QString entryName) :
    m_object(object), m_entryName(entryName)
{
    extractPath();
}

void RegistryEntry::setValue(const QPair<QString,QString>& pair) {
    m_object[pair.first] = pair.second;
}

QString RegistryEntry::value(const QString &key) const {
    return m_object[key];
}

QStringList RegistryEntry::print() {
    QStringList output;

//    QMap<QString, QString>::const_iterator i = m_object.constBegin();
//    while (i != m_object.constEnd()) {
//        output << QString(entryName() + " -- " + i.key() + ": " + i.value() + "\n");
//        ++i;
//    }

    output << QString(entryName() + " -- " + path() + "\n");

    return output;
}

void RegistryEntry::setEntryName(const QString& name) {
    m_entryName = name;
}

QString RegistryEntry::entryName() const {
    return m_entryName;
}

void RegistryEntry::setPath(const QString &path) {
    m_path = path;
}

QString RegistryEntry::path() const {
    return m_path;
}

void RegistryEntry::extractPath() {
    if (!m_object.contains(UNINSTALL_STRING))
        return ;

    QFileInfo fileInfo(m_object[UNINSTALL_STRING]);
    setPath(fileInfo.path().remove("\""));
}

}
