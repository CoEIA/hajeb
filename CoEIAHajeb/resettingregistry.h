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
#ifndef __RR__H
#define __RR__H

#include <windows.h>
#include <string>
#include <QString>
#include <QDir>
#include <QApplication>
#include <QDebug>

#define EXPLORER TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer")
#define HIDDEN TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\Folder\\Hidden\\SHOWALL")
#define REG	TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System")
#define MSCONFIG TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths")
#define DOS TEXT("Software\\Policies\\Microsoft\\Windows\\System")
#define STARTUP TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run")
#define CONTEXT TEXT("Folder\\shell")

#define HKLM HKEY_LOCAL_MACHINE
#define HKCU HKEY_CURRENT_USER
#define HKCR HKEY_CLASSES_ROOT

void createHajebPathKey();
void addToStartup(QString name);
void createHKLMDisabledTaskManagerKey();
void createHKCUDisabledTaskManagerKey();
bool SetValue(HKEY Root,LPTSTR SubKey ,LPTSTR valueName,DWORD value);
int ReadValue(HKEY Root,LPTSTR SubKey ,LPTSTR valueName);
bool isTaskManagerEnable();
bool enableTaskManager();
bool disableTaskManager();
bool deleteObject(const QString& path, const  QString& value);
QString ReadStringValue (HKEY Root,LPTSTR SubKey ,LPTSTR valueName );
QString readHajebPathKey();
void createInstallDirKey();
void createFilesPathKey();

#endif // __RR__H
