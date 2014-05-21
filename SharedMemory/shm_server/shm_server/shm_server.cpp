
/* 
	This program create a shared memory object and mutex object (in windows XP but also work for win7) and set the security to read/write for all.
	if we want to limit it to read change GWGR to GR.
*/

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <Sddl.h>


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

//int _tmain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE hMapFile;

	SECURITY_ATTRIBUTES security;
	ZeroMemory(&security, sizeof(security));
	security.nLength = sizeof(security);
	//ConvertStringSecurityDescriptorToSecurityDescriptor(L"D:P(A;OICI;GA;;;SY)(A;OICI;GA;;;BA)(A;OICI;GWGR;;;IU)",SDDL_REVISION_1,&security.lpSecurityDescriptor,NULL);
	ConvertStringSecurityDescriptorToSecurityDescriptor(L"D:P(A;OICI;GA;;;SY)(A;OICI;GA;;;BA)(A;OICI;GA;;;IU)",SDDL_REVISION_1,&security.lpSecurityDescriptor,NULL);

	hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,&security,PAGE_READWRITE,0,BUFF_SIZE,szName);
	if (hMapFile == NULL) 
	{ 
		_tprintf(TEXT("Could not create file mapping object (%d).\n"), GetLastError());
		return 1;
	}

   
	MySharedData* pSharedData = (MySharedData *) MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,BUFF_SIZE);           
 

	pSharedData->flag = SIG_NO_DATA;		// reset

	HANDLE ghMutex = CreateMutex(&security,FALSE,TEXT("Global\\MyMutexObject"));
	if (ghMutex == NULL) 
    {
        printf("CreateMutex error: %d\n", GetLastError());
        return 1;
    }

	LocalFree(security.lpSecurityDescriptor);

    //_getch();

	puts("Shared memory and mutex is created successfully");

	while (1) {
		Sleep(5000);
	}

	UnmapViewOfFile(pSharedData);
	CloseHandle(ghMutex);
	CloseHandle(hMapFile);

	return 0;
}

