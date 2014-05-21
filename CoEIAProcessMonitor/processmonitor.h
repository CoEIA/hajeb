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
