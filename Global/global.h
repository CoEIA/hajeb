#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

const QString SHARED_PATH_WINDOWS_XP	=   "C:\\Documents and Settings\\All Users\\Documents";
const QString SHARED_PATH_WINDOWS_VIST	=   "C:\\Users\\Public\\Documents";
const QString SHARED_PATH_WINDOWS_7	=   "C:\\Users\\Public\\Documents";

const QString HKLM_UNINSTALL_32   =     "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
const QString HKCU_UNINSTALL_32   =     "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
const QString HKLM_UNINSTALL_64   =     "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
const QString HKCU_UNINSTALL_64   =     "HKEY_CURRENT_USER\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

const QString PCSDB_NAME = "PCS-DB";
const QString LSPDB_NAME = "LSP-DB";

const QString server = "http://licenses.coeia.edu.sa/";
const QString local = "http://127.0.0.1/";
const QString URL = local;

extern QString INSTALL_PATH;
extern QString SHARED_PATH ;
extern QString DB_PATH;
extern QString LSPDB_PATH;
extern QString CONNECTCMD_FILE_PATH;
extern QString USER_ID_FILE_PATH;
extern QString SCREENSHOTS_PATH;

#endif // GLOBAL_H
