#include "resettingregistry.h"
#include "../Global/global.h"

void createHajebPathKey() {
    createInstallDirKey();
    createFilesPathKey();
}

void createInstallDirKey() {
    TCHAR value[MAX_PATH];
    QString path;
    QDir dir;
    dir.cdUp();
    path = dir.toNativeSeparators(dir.path());

    int length = path.toWCharArray(value);
    value[length] = '\0';

    QString name = "InstallDir";

    HKEY hNewKey ;
    DWORD Result ;
    RegCreateKeyEx(HKLM,TEXT("Software\\CoEIA\\Hajeb"),0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hNewKey,&Result);
    RegSetValueEx(hNewKey,name.toStdWString().c_str(),0,REG_SZ,(BYTE*)value,sizeof(value));
    RegCloseKey(hNewKey);
}

void createFilesPathKey() {
    TCHAR value[MAX_PATH];
    QString path = SHARED_PATH;
//    QString path;
//    QDir dir;
//    dir.cdUp();
//    path = dir.toNativeSeparators(dir.path());

    int length = path.toWCharArray(value);
    value[length] = '\0';

    QString name = "FilesPATH";

    HKEY hNewKey ;
    DWORD Result ;
    RegCreateKeyEx(HKLM,TEXT("Software\\CoEIA\\Hajeb"),0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hNewKey,&Result);
    RegSetValueEx(hNewKey,name.toStdWString().c_str(),0,REG_SZ,(BYTE*)value,sizeof(value));
    RegCloseKey(hNewKey);
}

QString readHajebPathKey() {
    return ReadStringValue(HKLM,TEXT("Software\\CoEIA\\Hajeb"),TEXT("FilesPATH"));
}

void addToStartup(QString name) {
    HMODULE hModule = GetModuleHandle(NULL);
    TCHAR ModulePath[MAX_PATH];
    TCHAR value[MAX_PATH];
    GetModuleFileName(hModule,ModulePath,sizeof(ModulePath));
    QString path = QDir::toNativeSeparators(qApp->applicationDirPath()) + "\\" + name + ".exe";
    int length = path.toWCharArray(value);
    value[length] = '\0';

    HKEY hNewKey ;
    DWORD Result ;
    RegCreateKeyEx(HKLM,STARTUP,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hNewKey,&Result);
    RegSetValueEx(hNewKey,name.toStdWString().c_str(),0,REG_SZ,(BYTE*)value,sizeof(value));
    RegCloseKey(hNewKey);
}

void createHKLMDisabledTaskManagerKey() {
    DWORD value = 1;

    HKEY hNewKey ;
    DWORD Result ;
    RegCreateKeyEx(HKLM,REG,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hNewKey,&Result);
    RegSetValueEx(hNewKey,TEXT("DisableTaskMgr"),0,REG_DWORD,(LPBYTE)&value,sizeof(value));
    RegCloseKey(hNewKey);
}

void createHKCUDisabledTaskManagerKey() {
    DWORD value = 1;

    HKEY hNewKey ;
    DWORD Result ;
    RegCreateKeyEx(HKCU,REG,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hNewKey,&Result);
    RegSetValueEx(hNewKey,TEXT("DisableTaskMgr"),0,REG_DWORD,(LPBYTE)&value,sizeof(value));
    RegCloseKey(hNewKey);
}

bool SetValue  (HKEY Root,LPTSTR SubKey ,LPTSTR valueName,DWORD value ) {
    HKEY hkey ;
    LONG lResult = RegOpenKeyEx(Root,SubKey,0,KEY_SET_VALUE,&hkey) ;

    if ( lResult == ERROR_SUCCESS ) {
        if (RegSetValueEx(hkey,valueName,0,REG_DWORD,(LPBYTE)&value,sizeof(value)) == ERROR_SUCCESS ) {
            RegCloseKey(hkey);
            return true;
        }
    }

    return false ;
}

int ReadValue (HKEY Root,LPTSTR SubKey ,LPTSTR valueName ) {
    HKEY hkey ;
    LONG lResult = RegOpenKeyEx(Root,SubKey,0,KEY_READ,&hkey) ;

    if ( lResult == ERROR_SUCCESS ) {
        DWORD dwSize = sizeof(DWORD);
        DWORD dwType ;
        DWORD dwValue ;
        lResult = RegQueryValueEx(hkey,valueName,0,&dwType,(LPBYTE)&dwValue,&dwSize);

        if ( lResult == ERROR_SUCCESS ) {
            RegCloseKey(hkey);
            return dwValue ;
        }
        else{
            if (lResult == ERROR_FILE_NOT_FOUND) {
                RegCloseKey(hkey);
                return 0 ;
            }
        }
    } else {
        if (lResult == ERROR_FILE_NOT_FOUND) {
            RegCloseKey(hkey);
            return 0 ;
        }
    }

    RegCloseKey(hkey);
    return 0 ;
}


bool isTaskManagerEnable() {
    return (ReadValue(HKCU,REG,TEXT("DisableTaskMgr"))? false : true);
}

bool enableTaskManager () {
    bool ret  = SetValue(HKLM,REG,TEXT("DisableTaskMgr"),0);
    bool ret2 = SetValue(HKCU,REG,TEXT("DisableTaskMgr"),0);

    if ( ret && ret2 )
        return true ;
    else
        return false ;
}

bool disableTaskManager () {
    bool ret  = SetValue(HKLM,REG,TEXT("DisableTaskMgr"),1);
    bool ret2 = SetValue(HKCU,REG,TEXT("DisableTaskMgr"),1);

    if ( ret && ret2 )
        return true ;
    else
        return false ;
}

bool deleteObject (const  QString& path, const  QString& value) {
    int index = path.indexOf("\\");
//    QString root = path.left(index);
    QString key = path.mid(index+1);
    HKEY hKey;
    if ( RegOpenKeyEx(HKLM, key.toStdWString().c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS ) {
        if ( RegDeleteValue(hKey, value.toStdWString().c_str()) != ERROR_SUCCESS ) {
            qDebug() << "cannot delete value" ;
            return (false);
         }
        RegCloseKey(hKey);
        return (true);
    }
    else {
         qDebug() << "not found key" ;
    }
    return (false);
}

QString ReadStringValue (HKEY Root,LPTSTR SubKey ,LPTSTR valueName ) {
    HKEY hkey ;
    LONG lResult = RegOpenKeyEx(Root,SubKey,0,KEY_READ,&hkey) ;
    TCHAR value[MAX_PATH];
    DWORD size = sizeof(value);

    if ( lResult == ERROR_SUCCESS ) {
        lResult = RegQueryValueEx(hkey,valueName,0,0,(BYTE*)value,&size);

        if ( lResult == ERROR_SUCCESS ) {
            RegCloseKey(hkey);
            return QString::fromWCharArray(value);
        }
        else{
            if (lResult == ERROR_FILE_NOT_FOUND) {
                RegCloseKey(hkey);
                return "" ;
            }
        }
    } else {
        if (lResult == ERROR_FILE_NOT_FOUND) {
            RegCloseKey(hkey);
            return "" ;
        }
    }

    RegCloseKey(hkey);
    return "" ;
}

