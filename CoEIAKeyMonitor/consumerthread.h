#ifndef CONSUMERTHREAD_H
#define CONSUMERTHREAD_H

#include <QThread>
#include <QSqlQuery>
#include <QSqlDatabase>

class ConsumerThread : public QThread
{
    Q_OBJECT
public:
    ConsumerThread(QObject* parent=0);
    void run();
private slots:
    void logText(QString text);
signals:
    void newText(QString text);
};

#endif // CONSUMERTHREAD_H
