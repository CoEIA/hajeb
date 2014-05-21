#include <QtGui/QApplication>
#include <QDir>
#include <QTextCodec>
#include "adminenablerwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    AdminEnablerWidget w;
    w.show();

    return a.exec();
}
