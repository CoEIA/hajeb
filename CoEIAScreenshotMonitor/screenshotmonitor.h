#ifndef SCREENSHOTMONITOR_H
#define SCREENSHOTMONITOR_H

#include <QObject>

class QTimer;

class ScreenshotMonitor : public QObject
{
    Q_OBJECT
public:
    ScreenshotMonitor(QObject* parent=0);
private slots:
    void timeout();
    void readTimePeriod();
private:
    QString getCurrentUserName(int userId);
    QString getCurrentUserName();
    int getCurrentUserId();
    int getCurrentUserId(QString userName);
    bool isEnabledForUser(int id);
    QTimer* m_timer;
    QTimer* m_screenshotTimePerdiod;
};

#endif // SCREENSHOTMONITOR_H
