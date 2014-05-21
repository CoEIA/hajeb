#include <QApplication>
#include <QList>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include "../../registryreader.h"
#include "../../registryentry.h"
#include "../../../Global/global.h"

using namespace ProgramLocker;

int main(int argc,char* argv[])
{
	QApplication app(argc,argv);
	
        QFile file("log");
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
            return -1;

        QTextStream out(&file);

        RegistryReader reader;

        QList<RegistryEntry> list;

        list << reader.read(HKLM_UNINSTALL_32);
        list << reader.read(HKCU_UNINSTALL_32);

        foreach (RegistryEntry obj, list) {
                foreach (QString line, obj.print()) {
                    out << line ;
                }
	}

	return 0;
}
