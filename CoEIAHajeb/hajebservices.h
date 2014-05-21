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
#ifndef HAJEBSERVICES_H
#define HAJEBSERVICES_H

#include <QString>

class HajebServices
{
public:
    static void install();
    static void uninstall();
    static void start();
    static void stop();

    static bool isServiceInstalled(QString serviceName) ;
    static bool isServiceRunning();
    static bool windows5ServicesStatus(QString serviceName);
    static bool windows6ServicesStatus(QString serviceName);

    static bool installLSP();
    static bool uninstallLSP();
    static QString lspGUID();

    // Windows 5: Windows XP, Windows Server 2003
    static void installHajebServicesWindows5();
    static void startHajebServicesWindows5();
    static void uninstallHajebServicesWindows5();
    static void stopHajebServicesWindows5();

    // Windows 6: Windows Vista, Windows Server 2008, Windows 7, Windows Server 2008 R2.
    static void installHajebServicesWindows6();
    static void startHajebServicesWindows6();
    static void uninstallHajebServicesWindows6();
    static void stopHajebServicesWindows6();
};

#endif // HAJEBSERVICES_H
