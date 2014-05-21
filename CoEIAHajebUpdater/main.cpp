#include <QApplication>
#include <QDir>
#include <QTextCodec>
#include "updatedialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    if (argc < 2)
        return -1;

    QStringList args = a.arguments();
    args.removeFirst();

    UpdateDialog dlg(args);
    dlg.exec();

    return 0;
}
