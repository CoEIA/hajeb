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
