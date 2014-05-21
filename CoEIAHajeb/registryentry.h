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
#ifndef REGISTRYOBJECT_H
#define REGISTRYOBJECT_H

#include <QMap>
#include <QPair>

namespace ProgramLocker {

class RegistryEntry
{
public:
    static const QString DISPLAY_NAME;
    static const QString DISPLAY_ICON;
    static const QString UNINSTALL_STRING;

    RegistryEntry(const QMap<QString, QString>& object, const QString entryName);
    void setEntryName(const QString& name);
    QString entryName() const;
    void setPath(const QString& path);
    QString path() const;
    void setValue(const QPair<QString,QString>& pair);
    QString value(const QString& key) const;
    QStringList print();
private:
    void extractPath();
    QMap<QString, QString> m_object;
    QString m_entryName;
    QString m_path;
};

}

#endif // REGISTRYOBJECT_H
