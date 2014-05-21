/* Orginal file coded by Wajdy Essam */

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
