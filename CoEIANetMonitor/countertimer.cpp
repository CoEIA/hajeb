#include "countertimer.h"

CounterTimer::CounterTimer(int userId,QObject* parent):
    QObject(parent)
{
    m_userId = userId;
}

int CounterTimer::hour() {
    return m_time.hour();
}

int CounterTimer::min() {
    return m_time.minute();
}

void CounterTimer::start() {

}

void CounterTimer::stop() {

}

void CounterTimer::updateTime() {

}
