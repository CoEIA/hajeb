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
