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
#ifndef APPLICATIONSTATUS_H
#define APPLICATIONSTATUS_H

#include <QObject>
#include <QDateTime>

class ApplicationStatus : public QObject
{
    Q_OBJECT
public:
    enum Status {
        TRIAL,
        BLOCK,
        END_TRIAL,
        FULL
    };

    ApplicationStatus(QObject* parent=0);
    Status applicationStatus();
    static void registerApplication(QString key,bool registerOnline);
    static bool isCleanVersion();
    static QDateTime installDate();
    static const int ALLOWED_DAYS;
    //static int allowedDays();
private slots:
    void updateCurrentTimeDate();
private:
    void readSettings();
    QDateTime m_installDateTime;
    QDateTime m_prevDateTime;
    bool m_isRegistered;
};

#endif // APPLICATIONSTATUS_H
