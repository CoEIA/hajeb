#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QTextCodec>
#include <QMessageBox>
#include <windows.h>

#include "removerwidget.h"
#include "../Global/global.h"

QString INSTALL_PATH;
QString SHARED_PATH ;
QString DB_PATH;
QString LSPDB_PATH;
QString CONNECTCMD_FILE_PATH;
QString USER_ID_FILE_PATH;
QString SCREENSHOTS_PATH;
QStringList getAllAdminsName();
QString getCurrentUserName();

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    if (QSysInfo::windowsVersion() == QSysInfo::WV_XP || QSysInfo::windowsVersion() == QSysInfo::WV_2003)
        SHARED_PATH = SHARED_PATH_WINDOWS_XP;
    else if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA)
        SHARED_PATH = SHARED_PATH_WINDOWS_VIST;
    else
        SHARED_PATH = SHARED_PATH_WINDOWS_7;

    QDir dir;
    dir.cdUp();
    INSTALL_PATH = dir.toNativeSeparators(dir.path());

    // Files PATH
    DB_PATH = INSTALL_PATH + "\\" + PCSDB_NAME;
    LSPDB_PATH = INSTALL_PATH + "\\" + LSPDB_NAME;
    CONNECTCMD_FILE_PATH = INSTALL_PATH + "\\connectcmd.txt";
    USER_ID_FILE_PATH = INSTALL_PATH + "\\userId.txt";
    SCREENSHOTS_PATH = INSTALL_PATH + "\\Screenshots";

//    QString currentUserName = getCurrentUserName();
//    QStringList adminNames = getAllAdminsName();
//    if (!adminNames.contains(currentUserName)) {
//        QMessageBox::critical(0,"لا يمكن تشغيل حاذف خدمات حاجب","بسبب عدم وجود صلاحيات كافية فانه لا يمكن تشغيل البرنامج.\nفقط مدير النظام هو المصرح له باستخدام هذا البرنامج.");
//        return -1 ;
//    }
    RemoverWidget* rmWidget = new RemoverWidget;
    rmWidget->show();

    return app.exec();
}

QStringList getAllAdminsName() {
    QStringList userName ;
    QProcess* process = new QProcess;
    process->start("net localgroup administrators");
    process->waitForStarted();
    process->waitForFinished();
    QString output = process->readAll();
    QStringList lines = output.split("\n");
    int i = 0;
    QString line = lines.at(i);
    while (!line.contains("-----------------"))
        line = lines.at(++i);
    ++i;
    while (i < lines.size()-3)
        userName << lines.at(i++).trimmed();
    return userName;
}

QString getCurrentUserName() {
    TCHAR buffer[100];
    DWORD buffer_size = 100;
    GetUserName(buffer,&buffer_size);
    return QString::fromWCharArray(buffer);
}
