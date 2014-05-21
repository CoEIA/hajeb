#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <QObject>

class QTimer;
class QProcess;

class ProcessMonitor : public QObject
{
    Q_OBJECT
public:
    ProcessMonitor(QObject* parent=0);
private slots:
    void timeout();
private:
    QString getCurrentUserName(int userId);
    QString getCurrentUserName();
    int getCurrentUserId(QString userName);
    int getCurrentUserId();
    bool isEnabledForUser(int id);
    QStringList getProcessName(int userId);
    void logs(int userId,QString action);
    QTimer* m_timer;
    QProcess* m_process;
};

#endif // PROCESSMONITOR_H
