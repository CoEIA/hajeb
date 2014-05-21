#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include "updatedialog.h"
#include "ui_updatedialog.h"
#include "../Global/global.h"

UpdateDialog::UpdateDialog(QStringList list,QWidget *parent):
    QDialog(parent),ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);
    updateList = list;

    exeName["Version"] = "version.txt";
    exeName["CoEIA_Hajeb_Control_Panel"] = "Hajeb.exe";
    exeName["CoEIA_LSP_Filter"] = "NonIFSLSP.dll";
    exeName["CoEIA_Keylogger"] = "CoEIAKeyMonitor.exe";
    exeName["CoEIA_Process_Monitor"] = "CoEIAProcessMonitor.exe";
    exeName["CoEIA_Screenshot_Monitor"] = "CoEIAScreenshotMonitor.exe";
    exeName["CoEIA_Network_Monitor"] = "CoEIANetMonitor.exe";
    exeName["CoEIA_Hajeb_Remover"] = "HajebRemover.exe";

    startDownload();
    startHajeb();
    updateVersionFile();
    ui->progressBar->setValue(100);
    ui->label->setText("انتهت عملية التحديث بنجاح");
    QTimer::singleShot(500,this,SLOT(accept()));
}

UpdateDialog::~UpdateDialog() {
    delete ui;
}

void UpdateDialog::download(QString fileName) {
    QProcess* process = new QProcess;
    QString programName = "\"wget.exe\"";
    QString output = "-Onew_" + exeName[fileName] ;
    QString parameter = URL+"software/hajeb/bin/" + exeName[fileName] ;
    process->start(programName,QStringList() << output << parameter);
    process->waitForStarted();
    process->waitForFinished();
    delete process;
}

bool UpdateDialog::remove(QString fileName) {
    QDir dir;
    return dir.remove(exeName[fileName]);
}

bool UpdateDialog::rename(QString fileName) {
    QDir dir;
    return dir.rename("new_"+exeName[fileName],exeName[fileName]);
}

bool UpdateDialog::isExist(QString fileName) {
    QDir dir;
    return dir.exists(exeName[fileName]);
}

void UpdateDialog::startDownload() {
    int count = updateList.count();
    int value = 100 / count;

    foreach (QString file,updateList) {
        ui->label->setText(QString("تحميل الملف: %1").arg(file));
        download(file);
        qApp->processEvents();
        ui->progressBar->setValue(ui->progressBar->value()+value);
        if (isExist(file)) {
            qDebug() << "old files exist.";
            if (remove(file)) {
                qApp->processEvents();
                if (rename(file)) {
                    ui->label->setText(QString("تم تنصيب الملف: %1 بنجاح").arg(file));
                } else
                    QMessageBox::warning(this,"خطأ في عملية التحديث","لا يمكن تعديل اسم الملف");
            } else
                QMessageBox::warning(this,"خطأ في عملية التحديث","لا يمكن حذف ملف النسخة السابقة");
        } else {
             qDebug() << "old file not exist.";
            if (rename(file)) {
                ui->label->setText(QString("تم تنصيب الملف: %1 بنجاح").arg(file));
            } else
                QMessageBox::warning(this,"خطأ في عملية التحديث","لا يمكن تعديل اسم الملف");
        }
    }
    qApp->processEvents();
}

void UpdateDialog::startHajeb() {
    QProcess* m_process = new QProcess;
    QString pName = "\"Hajeb.exe\"";
    m_process->start(pName,QStringList() << "-auto");
}

void UpdateDialog::updateVersionFile() {
    qApp->processEvents();
    download("Version");
    qApp->processEvents();
    if (remove("Version")) {
        rename("Version");
    } else
        QMessageBox::warning(this,"خطأ في عملية التحديث","لا يمكن حذف ملف النسخة");
}
