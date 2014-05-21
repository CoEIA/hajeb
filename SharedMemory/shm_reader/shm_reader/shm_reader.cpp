
/* 
	This program read a text from the shared memory.
*/

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <Sddl.h>
#include <iostream>
#include "sqlite3.h"
#include <fstream>

TCHAR szName[]=TEXT("Global\\LSPSHMM");


#define SIG_NO_DATA		0
#define SIG_DATA		1

#define DOMAIN_SIZE		100
#define BUFF_SIZE		4000
#define URL_SIZE		3000

#define HKLM HKEY_LOCAL_MACHINE

std::string dbPath;
std::string userIdPath;

std::string path;

enum MSG_TYPE { USER_LOG, WEB_LOG };

struct MySharedData
{
    unsigned char flag;
	MSG_TYPE type;
	int userId;
    unsigned char domain[DOMAIN_SIZE];
	unsigned char url[URL_SIZE];
	bool log;
};

std::string ReadStringValue (HKEY Root,LPTSTR SubKey ,LPTSTR valueName);
std::string ws2s(const std::wstring& s);
std::string readHajebPathKey();
std::string readHajebInstallDirKey();
void readPath();

//int _tmain()  //change linker system option to console..
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	HANDLE hMapFile;
	hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,szName);
	if (hMapFile == NULL) 
	{ 
      _tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
      return 1;
	} 
 
	MySharedData* pSharedData = (MySharedData *) MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,BUFF_SIZE);                   
 
	HANDLE ghMutex; 
	ghMutex = OpenMutex( MUTEX_ALL_ACCESS,FALSE,TEXT("Global\\MyMutexObject"));
    if (ghMutex == NULL) {
        printf("OpenMutex error: %d\n", GetLastError() );
		return 1;
	}

	DWORD dwWaitResult; 

	int userId;
	MSG_TYPE type;
	bool log;

	readPath();

	// Open Database
	sqlite3 *db;
	if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
		return 1;
	}


	do 
	{

		puts("Waiting for data ..");

		while (pSharedData->flag == SIG_NO_DATA) { // busy wait for data.
			//puts("SIG_NO_DATA");
			Sleep(100);
			;	
		}

		// acquire the mutex
		dwWaitResult = WaitForSingleObject( 
            ghMutex,    // handle to mutex
            INFINITE);  // no time-out interval

		puts("Inside critical section");
		// read data
		printf("Type: %d\n",pSharedData->type);
		printf("UserId: %d\n",pSharedData->userId);
		printf("Domain: %s\n",pSharedData->domain);
		printf("Url: %s\n",pSharedData->url);

		type = pSharedData->type;
		userId = pSharedData->userId;
		char* dStr = new char[sizeof(pSharedData->domain)+1];
		strncpy(dStr,(const char*)pSharedData->domain,sizeof(pSharedData->domain));
		char* uStr = new char[sizeof(pSharedData->url)+1];
		strncpy(uStr,(const char*)pSharedData->url,sizeof(pSharedData->url));
		log = pSharedData->log;

		// reset the flag.
		pSharedData->flag = SIG_NO_DATA;

		// release the mutex
		ReleaseMutex(ghMutex);

		puts("Outside critical section");

		if (type == USER_LOG) {
			// Save id in userId.txt
			puts("Save userId");
			std::ofstream myfile;
			myfile.open(userIdPath, std::ios::out | std::ios::trunc);
			myfile <<  userId;
			myfile.close();

		} else {
			sqlite3_stmt *stm;
			// Save data in DB.
			if (log) 
			{
				puts("browse log");
				char* query2 = "INSERT INTO BrowseLogsTable(userId,domain,url,currentDate,currentTime) VALUES (?,?,?,date(\'now\',\'localtime\'),time(\'now\',\'localtime\'));";
				sqlite3_prepare_v2(db,query2,-1,&stm,0);
				sqlite3_bind_int(stm,1,userId);
				sqlite3_bind_text(stm, 2, dStr, -1, SQLITE_TRANSIENT);
				sqlite3_bind_text(stm, 3, uStr, -1, SQLITE_TRANSIENT);
			}
			else
			{
				puts("blocked site log");
				char* query2 = "INSERT INTO LogsTable(userId,action,currentDate,currentTime,actionType) VALUES (?,?,date(\'now\',\'localtime\'),time(\'now\',\'localtime\'),1);";
				sqlite3_prepare_v2(db,query2,-1,&stm,0);
				sqlite3_bind_int(stm,1,userId);
				std::string action = "Denied Access to website ";
				action.append(dStr) ;
				sqlite3_bind_text(stm, 2, action.c_str() , -1, SQLITE_TRANSIENT);
			}

			sqlite3_step(stm);
			sqlite3_finalize(stm);
		}

		delete [] dStr;
		delete [] uStr;
		
	} while(1);

	//CopyMemory((PVOID)pBuf, szMsg, (_tcslen(szMsg) * sizeof(TCHAR)));
	//MessageBox(NULL, pBuf, TEXT("Process2"), MB_OK);

    _getch();


	sqlite3_close(db);
	UnmapViewOfFile(pSharedData);
	CloseHandle(ghMutex);
	CloseHandle(hMapFile);
	return 0;
}

std::string ws2s(const std::wstring& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
    char* buf = new char[len];
    WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
    std::string r(buf);
    delete[] buf;
    return r;
}

std::string ReadStringValue (HKEY Root,LPTSTR SubKey ,LPTSTR valueName ) {
    HKEY hkey ;
    LONG lResult = RegOpenKeyEx(Root,SubKey,0,KEY_READ,&hkey) ;
    TCHAR value[MAX_PATH];
    DWORD size = sizeof(value);

    if ( lResult == ERROR_SUCCESS ) {
        lResult = RegQueryValueEx(hkey,valueName,0,0,(BYTE*)value,&size);
        if ( lResult == ERROR_SUCCESS ) {
            RegCloseKey(hkey);
			return value;

            //return ws2s(value);
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

std::string readHajebPathKey() {
    return ReadStringValue(HKLM,TEXT("Software\\CoEIA\\Hajeb"),TEXT("FilesPATH"));
}

std::string readHajebInstallDirKey() {
    return ReadStringValue(HKLM,TEXT("Software\\CoEIA\\Hajeb"),TEXT("InstallDir"));
}

void readPath() {
	dbPath = readHajebInstallDirKey();
	userIdPath = dbPath;

	dbPath.append("\\LSP-DB");
	userIdPath.append("\\userId.txt");
}