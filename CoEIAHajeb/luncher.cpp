#include <QProcess>
#include "luncher.h"

void Luncher::lunchUpdate(QStringList files) {
    QProcess* m_process = new QProcess;
    QString pName = "\"CoEIAHajebUpdater.exe\"";
    m_process->start(pName,files);
}
