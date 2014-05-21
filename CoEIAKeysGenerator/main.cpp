#include <QtGui/QApplication>
#include <QtSql>
#include "keysgeneratorwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    qDebug() << QSqlDatabase::drivers();
    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
    database.setDatabaseName("coeia_hajeb");
    database.setHostName("localhost");
    database.setUserName("root");
    database.setPassword("");

    if ( ! database.open() ) {
        qDebug() << "Error: " << database.lastError().text();
        return -1;
    }

    KeysGeneratorWidget w;
    w.show();
    return a.exec();
}
