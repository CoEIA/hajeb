
/* 
	This program write a text to the shared memory.
*/

#include "stdafx.h"


#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "user32.lib")


TCHAR szName[]=TEXT("Global\\LSPSHMM");


#define SIG_NO_DATA		0
#define SIG_DATA		1
#define DOMAIN_SIZE		100
#define BUFF_SIZE		4000
#define URL_SIZE		3000

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

int _tmain()
{
	HANDLE hMapFile;
	hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,szName);
	if (hMapFile == NULL) 
	{ 
		_tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
		return 1;
	} 
	
	MySharedData* pSharedData = (MySharedData *) MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,BUFF_SIZE);                   
	
	HANDLE hMutex; 
	hMutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,TEXT("Global\\MyMutexObject"));
	if (hMutex == NULL) {
        printf("OpenMutex error: %d\n", GetLastError() );
		return 1;
	}
    //else 
		//printf("OpenMutex successfully opened the mutex.\n");

	int userId = 1;
	MSG_TYPE type = WEB_LOG;
	std::string domain = "http://www.qt-ar.org";
	std::string url = "http://www.google.com/search?q=QMutex&ie=utf-8&oe=utf-8&aq=t&rls=org.mozilla:en-US:official&client=firefox-a";
	bool log = false;

	puts("Press [Enter] to write a message to shared memory");
	_getch();
	do
	{
	   // acquire the mutex.
	   WaitForSingleObject(hMutex,INFINITE); 

	   // set data
	   //size_t len = sizeof(pSharedData->buff);
       //memset (pSharedData->buff, 0xFF, len);
	   pSharedData->type = type;
	   pSharedData->userId = userId;
	   CopyMemory(pSharedData->domain, domain.c_str(), domain.length());
	   CopyMemory(pSharedData->url, url.c_str(), url.length());
	   pSharedData->log = log;

	   // set flag
	   pSharedData->flag = SIG_DATA;

	   // release the mutex.
	   ReleaseMutex(hMutex);

	   puts("Press [Enter] to write a message to shared memory");
	   _getch();
	} while (1);

	//CopyMemory((PVOID)pBuf, szMsg, (_tcslen(szMsg) * sizeof(TCHAR)));

	_getch();

	CloseHandle(hMutex);
	UnmapViewOfFile(pSharedData);
	CloseHandle(hMapFile);
 
	return 0;
}