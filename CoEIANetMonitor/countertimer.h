#ifndef COUNTERTIMER_H
#define COUNTERTIMER_H

#include <QObject>
#include <QTime>

class CounterTimer : public QObject
{
    Q_OBJECT
public:
    CounterTimer(int userId,QObject* parent=0);
    int hour();
    int min();
public slots:
    void start();
    void stop();
    void updateTime();
private:
    QTime m_time;
    int m_userId;
};

#endif // COUNTERTIMER_H
