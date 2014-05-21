#include <QProcess>
#include "hajebservices.h"
#include "resettingregistry.h"

#ifdef QT_NO_DEBUG
    #define INSTALL_SERVICES 1
    #define INSTALL_LSP 1
#else
    #define INSTALL_SERVICES 0
    #define INSTALL_LSP 0
#endif

void HajebServices::install() {
#if INSTALL_SERVICES
    if (QSysInfo::windowsVersion() == QSysInfo::WV_2003 || QSysInfo::windowsVersion() == QSysInfo::WV_XP) {
        // Install Hajeb Process as Services
        // The problem with Windows v6 is that services run in different session.
        // Service isolation in session 0, an application run in session 1,2...etc.

        installHajebServicesWindows5();
        startHajebServicesWindows5();
        addToStartup("CoEIAUserNameMonitor");

    } else {
        /* Duo to the problem of sevices, There is a two solution
            1- Disable Task Manager. So user Cannot kill Hajeb process.
            2- Create a Service in session0 that enumerate session and create process as user
                so We can run our process as SYSTEM.

            ** This Code now support solution 2.
           */

        installHajebServicesWindows6();
        startHajebServicesWindows6();
        addToStartup("CoEIAUserNameMonitor");
    }

#else
    qDebug() << "No SERVICES: No SERVICES Installation" ;
#endif

}

void HajebServices::uninstall() {
#if INSTALL_SERVICES
    if (QSysInfo::windowsVersion() == QSysInfo::WV_2003 || QSysInfo::windowsVersion() == QSysInfo::WV_XP) {
        stopHajebServicesWindows5();
        uninstallHajebServicesWindows5();
    }
    else {
        stopHajebServicesWindows6();
        uninstallHajebServicesWindows6();
    }
#endif
}

void HajebServices::installHajebServicesWindows5() {
    QProcess* m_process = new QProcess;
    QString pName = "\"XYNTService.exe\"";
    m_process->start(pName,QStringList() << "-i");
    if (!m_process->waitForStarted())
        qDebug() << "Unable to Install services.";
    if (!m_process->waitForFinished())
        qDebug() << "Services can not finish installation.";
    QString output = m_process->readAll();
    qDebug() << output;
    delete m_process;
    m_process = 0;
}

void HajebServices::uninstallHajebServicesWindows5() {
    // Uninstall Services
    QProcess* m_process = new QProcess;
    QString pName = "\"XYNTService.exe\"";
    m_process->start(pName,QStringList() << "-u");
    m_process->waitForStarted();
    m_process->waitForFinished();

    delete m_process;
    m_process = 0;

    // Remove Registery key: CoEIAUserNameMonitor
    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAUserNameMonitor");

}

void HajebServices::startHajebServicesWindows5() {
    QProcess* m_process = new QProcess;
    QString pName = "\"net.exe\"";
    m_process->start(pName,QStringList() << "start" << "CoEIA-Hajeb");
    if (!m_process->waitForStarted())
        qDebug() << "Unable to Start Services.";
    if (!m_process->waitForFinished())
        qDebug() << "Services can not Start.";
    delete m_process;
    m_process = 0;
}

void HajebServices::stopHajebServicesWindows5() {
    // Stop Services.
    QProcess* m_process = new QProcess;
    QString pName = "\"net.exe\"";
    m_process->start(pName,QStringList() << "stop" << "CoEIA-Hajeb");
    m_process->waitForStarted();
    m_process->waitForFinished();
    delete m_process;
    m_process = 0;
}

void HajebServices::installHajebServicesWindows6() {
//    addToStartup("CoEIAUserNameMonitor");
//    addToStartup("CoEIAScreenshotMonitor");
//    addToStartup("CoEIAProcessMonitor");
//    addToStartup("CoEIAKeyMonitor");
//    addToStartup("CoEIANetMonitor");

//    // Disable Task Manager
//    if (!disableTaskManager()) {
//        createHKLMDisabledTaskManagerKey();
//        createHKCUDisabledTaskManagerKey();
//    }

    QProcess* m_process = new QProcess;
    QString pName = "\"Service1.exe\"";
    m_process->start(pName,QStringList() << "-i");
    if (!m_process->waitForStarted())
        qDebug() << "Unable to Install services.";
    if (!m_process->waitForFinished())
        qDebug() << "Services can not finish installation.";
    QString output = m_process->readAll();
    qDebug() << output;
    delete m_process;
    m_process = 0;
}

void HajebServices::uninstallHajebServicesWindows6() {
//    // Remove Hajeb Registery keys:
//    // CoEIAUserNameMonitor, CoEIAScreenshotMonitor, CoEIAProcessMonitor, CoEIAKeyMonitor, CoEIANetMonitor
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAUserNameMonitor");
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAScreenshotMonitor");
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAProcessMonitor");
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAKeyMonitor");
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIANetMonitor");

//    // Enable Task Manager
//    enableTaskManager();

    // Uninstall Services
    QProcess* m_process = new QProcess;
    QString pName = "\"Service1.exe\"";
    m_process->start(pName,QStringList() << "-d");
    m_process->waitForStarted();
    m_process->waitForFinished();

    delete m_process;
    m_process = 0;

    // Remove Registery key: CoEIAUserNameMonitor
    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAUserNameMonitor");
}

void HajebServices::startHajebServicesWindows6() {
//    QDir dir;
//    QStringList hajebServices;
//    hajebServices << "CoEIAUserNameMonitor" << "CoEIAScreenshotMonitor" << "CoEIAProcessMonitor" << "CoEIAKeyMonitor" << "CoEIANetMonitor";
//    foreach (QString program,hajebServices) {
//        QProcess* process = new QProcess;
//        program.append(".exe\"");
//        program.prepend("\"" + QDir::toNativeSeparators(dir.absolutePath()) + "\\");
//        process->start(program);
//        if (!process->waitForStarted())
//            qDebug() << "Unable to start" << process->errorString();
//    }

    QProcess* m_process = new QProcess;
    QString pName = "\"net.exe\"";
    m_process->start(pName,QStringList() << "start" << "Hajeb7Svc");
    if (!m_process->waitForStarted())
        qDebug() << "Unable to Start Services.";
    if (!m_process->waitForFinished())
        qDebug() << "Services can not Start.";
    delete m_process;
    m_process = 0;

    // Send message to Service to start hajeb process.
    m_process = new QProcess;
    pName = "\"CustomMessageSender.exe\"";
    m_process->start(pName);
    m_process->waitForStarted();
    m_process->waitForFinished();
    delete m_process;
    m_process = 0;
}

void HajebServices::stopHajebServicesWindows6() {
//    // Kill Hajeb Services
    QStringList pKillNames;
    pKillNames << "CoEIAScreenshotMonitor" << "CoEIAProcessMonitor" << "CoEIAKeyMonitor" << "CoEIANetMonitor" << "shm_reader";

    foreach (QString pKillName,pKillNames) {
        pKillName.append(".exe");
        QString pName = QString("taskkill /im %1 /f").arg(pKillName);
        QProcess* process = new QProcess;
        process->start(pName);
        if (!process->waitForStarted())
            qDebug() << process->errorString();
        if (!process->waitForFinished())
            qDebug() << process->errorString();
        delete process;
        process = 0;
    }

    // Stop Services.
    QProcess* m_process = new QProcess;
    QString pName = "\"net.exe\"";
    m_process->start(pName,QStringList() << "stop" << "Hajeb7Svc");
    m_process->waitForStarted();
    m_process->waitForFinished();
    delete m_process;
    m_process = 0;
}

bool HajebServices::isServiceInstalled(QString serviceName) {
    QProcess* process = new QProcess;
    process->start("sc.exe query " + serviceName);
    process->waitForStarted();
    process->waitForFinished();
    QString output = process->readAll();
    delete process;
    QStringList lines = output.split("\n");
    if (lines.at(0).contains("FAILED"))
        return false;
    else
        return true;
}

bool HajebServices::isServiceRunning() {
    if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA ||
        QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7)
        return windows6ServicesStatus("Hajeb7Svc");
    else
        return windows5ServicesStatus("CoEIA-Hajeb");
}

bool HajebServices::windows5ServicesStatus(QString serviceName) {
    QProcess* process = new QProcess;
    process->start("sc.exe query " + serviceName);
    process->waitForStarted();
    process->waitForFinished();
    QString output = process->readAll();
    delete process;
    QStringList lines = output.split("\n");
    if (lines.at(0).contains("FAILED"))
        return false;
    if (lines.at(3).contains("RUNNING"))
        return true;
    else
        return false;
}

bool HajebServices::windows6ServicesStatus(QString serviceName) {
//    QProcess* process = new QProcess;
//    process->start("tasklist /FI \"IMAGENAME eq CoEIANetMonitor.exe\"");
//    process->waitForStarted();
//    process->waitForFinished();
//    QString output = process->readAll();
//    delete process;
//    if (output.contains("INFO: No"))
//        return false;
//    return true;

    QProcess* process = new QProcess;
    process->start("sc.exe query " + serviceName);
    process->waitForStarted();
    process->waitForFinished();
    QString output = process->readAll();
    delete process;
    QStringList lines = output.split("\n");
    if (lines.at(0).contains("FAILED"))
        return false;
    if (lines.at(3).contains("RUNNING"))
        return true;
    else
        return false;
}

bool HajebServices::installLSP() {
    bool install = false;
#if INSTALL_LSP
    // Install LSP.
    QProcess* m_process = new QProcess;
    QString pName = "\"Install LSP.BAT\"";
    m_process->start(pName,QStringList());
    if (!m_process->waitForStarted())
        qDebug() << "Unable to Install LSP";
    if (!m_process->waitForFinished())
        qDebug() << "LSP can not Start.";
    QString output = m_process->readAll();
    qDebug() << output;
    delete m_process;
    m_process = 0;
    install = true;
#else
    qDebug() << "NO_LSP: No LSP Installation" ;
#endif

    return install;
}

bool HajebServices::uninstallLSP() {
#if !INSTALL_LSP
    return false;
#else
    // Get LSP guid
    QString guid = lspGUID();
    qDebug() << guid ;

    if (guid.isEmpty())
        return false;

    // Execute RegisterLSP.exe -r guid
    QProcess* process = new QProcess;
    QString programName = "\"RegisterLSP.exe\"";
    QString parameter = "-r";
    process->start(programName,QStringList() << parameter << guid);
    process->waitForStarted();
    process->write(" ");
    process->waitForFinished();
    delete process;
    return true;
#endif
}

QString HajebServices::lspGUID() {
    QProcess* process = new QProcess;
    QString programName = "\"RegisterLSP.exe\"";
    QString parameter = "-p";
    process->start(programName,QStringList() << parameter);
    if (!process->waitForStarted())
        return "" ;
    process->waitForFinished();

    QString guid = process->readAll();
    delete process;
    process = 0;

    QStringList list = guid.split("\n");
    list.removeAll("");
    guid = list.last();
    QStringList provider = guid.split('-');
    QString providerName = provider.at(1);
    QString providerId = provider.at(0);
    providerName = providerName.trimmed();
    providerId = providerId.trimmed();
    if (!providerName.contains("CoEIALSP"))
        return QString();
    return providerId;
}
