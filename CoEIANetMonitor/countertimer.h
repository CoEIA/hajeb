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
