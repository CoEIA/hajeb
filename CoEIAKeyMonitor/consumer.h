#ifndef CONSUMER_H
#define CONSUMER_H

#include <QObject>

class QTimer;

class Consumer : public QObject
{
    Q_OBJECT
public:
    Consumer(QObject* parent=0);
private slots:
    void consume();
private:
    bool isEnabledForUser(int id);
    int getCurrentUserId();
    QTimer* m_timer;
};

#endif // CONSUMER_H
