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
#include <QApplication>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include "removerwidget.h"
#include "ui_removerwidget.h"
#include "resettingregistry.h"
#include "../Global/global.h"

bool removeLSP();
QString lspGUID();

void removeWindows5Settings();
void removeWindows6Settings();

RemoverWidget::RemoverWidget(QWidget *parent):
        QWidget(parent),ui(new Ui::RemoverWidget)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
}

RemoverWidget::~RemoverWidget() {
    delete ui;
    ui = 0 ;
}

void RemoverWidget::on_removeButton_clicked() {
    ui->cancelButton->setEnabled(false);
    ui->removeButton->setEnabled(false);

    ui->progressBar->setValue(30);
    ui->operationLabel->setText("... حذف ملفتر حاجب");
    qApp->processEvents();

    // Remove LSP in all windows version
    removeLSP();

    ui->progressBar->setValue(50);
    ui->operationLabel->setText("... حذف الخدمات");
    qApp->processEvents();

    // Remove Hajeb Services
    switch (QSysInfo::windowsVersion()) {
    case QSysInfo::WV_XP:
        removeWindows5Settings();
        break;
    case QSysInfo::WV_VISTA:
    case QSysInfo::WV_WINDOWS7:
        removeWindows6Settings();
        break;
    default:
        break;
    }

    ui->progressBar->setValue(50);
    ui->operationLabel->setText("... حذف الملفات المؤقتة");
    qApp->processEvents();

    // Remove Output files
    QDir dir;
    dir.remove("XYNTService.log");
    dir.remove("lsp.txt");

    ui->progressBar->setValue(75);

    ui->operationLabel->setText("... حذف قاعدة البيانات");
    // Remove Database and  Control file
    dir.setPath(INSTALL_PATH);
    dir.remove(PCSDB_NAME);
    dir.remove(LSPDB_NAME);
    dir.remove("connectcmd.txt");
    dir.remove("userId.txt");

    //dir.setPath(SHARED_PATH);

    ui->progressBar->setValue(90);
    ui->operationLabel->setText("... حذف لقطات الشاشة");
    qApp->processEvents();

    // Remove Screenshots Files
    QString path = INSTALL_PATH + "\\Screenshots";
    QDir screenshotsDir;
    screenshotsDir.setPath(path);
    foreach (QFileInfo dirName,screenshotsDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot)) {
        QDir userNameDir = path + "\\" + dirName.fileName();
        foreach (QFileInfo fileInfo,userNameDir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot)) {
            qDebug() << "remove " << fileInfo.fileName() << " : " << screenshotsDir.remove(fileInfo.absoluteFilePath());
        }
        qDebug() << "remove " << dirName.fileName() << " : " << screenshotsDir.rmdir(dirName.fileName());
    }
    qDebug() << "remove Screenshots: " << screenshotsDir.rmdir("Screenshots");


    ui->progressBar->setValue(100);
    ui->operationLabel->setText("<b><font color=green>تم حذف الإعدادات والخدمات. يمكنك حذف واجهة حاجب من خلال لوحة التحكم : اضافة او الزالة البرامج</font></b>");
    qApp->processEvents();

    ui->cancelButton->setEnabled(true);
    ui->cancelButton->setIcon(QIcon(":/images/ok.png"));
    ui->cancelButton->setText("انهاء");
}

void RemoverWidget::on_cancelButton_clicked() {
    qApp->quit();
}

bool removeLSP() {
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
}

QString lspGUID() {
//    QFile file("lsp.txt");
//    if (!file.open(QIODevice::ReadOnly)) {
//        return QString();
//    }

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

// Windows XP, Windows Server 2003
void removeWindows5Settings() {
    // Stop Services.
    QProcess* m_process = new QProcess;
    QString pName = "\"net.exe\"";
    m_process->start(pName,QStringList() << "stop" << "CoEIA-Hajeb");
    m_process->waitForStarted();
    m_process->waitForFinished();
    delete m_process;
    m_process = 0;

    // Uninstall Services
    m_process = new QProcess;
    pName = "\"XYNTService.exe\"";
    m_process->start(pName,QStringList() << "-u");
    m_process->waitForStarted();
    m_process->waitForFinished();

    delete m_process;
    m_process = 0;

    // Enable Task Manager
    enableTaskManager();

    // Remove Registery key: CoEIAUserNameMonitor
    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAUserNameMonitor");
}

// Windows Vista, Windows Server 2008, Windows 7, Windows Server 2008 R2.
void removeWindows6Settings() {
//    // Remove Hajeb Registery keys:
//    // CoEIAUserNameMonitor, CoEIAScreenshotMonitor, CoEIAProcessMonitor, CoEIAKeyMonitor, CoEIANetMonitor
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAUserNameMonitor");
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAScreenshotMonitor");
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAProcessMonitor");
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAKeyMonitor");
//    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIANetMonitor");

    // Enable Task Manager
    enableTaskManager();

    // Kill Hajeb Services
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


    // Uninstall Services
    m_process = new QProcess;
    pName = "\"Service1.exe\"";
    m_process->start(pName,QStringList() << "-d");
    m_process->waitForStarted();
    m_process->waitForFinished();

    delete m_process;
    m_process = 0;

    // Remove Registery key: CoEIAUserNameMonitor
    deleteObject("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", "CoEIAUserNameMonitor");
}
