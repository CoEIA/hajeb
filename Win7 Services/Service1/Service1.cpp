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
#include "stdafx.h"
#include "Service1.h"
#include "Windows.h"
#include "stdafx.h"
#include "Winsvc.h"
#include "time.h"
#include <WtsApi32.h>
#include <UserEnv.h>
#include <tlhelp32.h>
#include <Sddl.h>
#include <iostream>
#include <string>

#pragma comment(lib,"WtsApi32.lib")
#pragma comment(lib,"UserEnv.lib")


/////////////////////////////////////////////////////////////////////////////
//Define custom message added by Jaisvar
/////////////////////////////////////////////////////////////////////////////

//CUSTOM MESSAGE FOR SERVICE TO LAUNCH AN APP INTO SESSION 1 
#define SERVICE_CONTROL_CUSTOM_MESSAGE 0x0096

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//Method added by Jaisvar
/////////////////////////////////////////////////////////////////////////////
BOOL LaunchAppIntoDifferentSession();

/////////////////////////////////////////////////////////////////////////////

/***************** Ahmed Isam ***************/
std::string path;
std::string dbPath;
std::string keyMonitorPath;
std::string processMonitorPath;
std::string screenshotMonitorPath;
std::string netMonitorPath;
std::string serverPath;

#define HKLM HKEY_LOCAL_MACHINE

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
    return ReadStringValue(HKLM,TEXT("Software\\CoEIA\\Hajeb"),TEXT("InstallDir"));
}

void readPath() {
	// read registry key
	path = readHajebPathKey();
	dbPath = path + "\\LSP-DB";
	keyMonitorPath = path + "\\bin\\CoEIAKeyMonitor.exe";
	processMonitorPath = path + "\\bin\\CoEIAProcessMonitor.exe";
	screenshotMonitorPath = path + "\\bin\\CoEIAScreenshotMonitor.exe";
	netMonitorPath = path + "\\bin\\CoEIANetMonitor.exe";
	serverPath = path + "\\bin\\shm_reader.exe";
}

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
/**********************************************************/


SERVICE_STATUS m_ServiceStatus;
SERVICE_STATUS_HANDLE m_ServiceStatusHandle;
BOOL bRunning=true;
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
void WINAPI ServiceCtrlHandler(DWORD Opcode);
BOOL InstallService();
BOOL DeleteService();


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	readPath();

  if(argc>1)
  {
    if(strcmp(argv[1],"-i")==0)
    {
      if(InstallService())
        printf("\n\nService Installed Sucessfully\n");
      else
        printf("\n\nError Installing Service\n");
    }
    else if(strcmp(argv[1],"-d")==0)
    {
      if(DeleteService())
        printf("\n\nService UnInstalled Sucessfully\n");
      else
        printf("\n\nError UnInstalling Service\n");
    }
    else
    {
      printf("\n\nUnknown Switch Usage\n\nFor Install use Service1 -i\n\nFor UnInstall use Service1 -d\n");
    }
  }
  else
  {
    SERVICE_TABLE_ENTRY DispatchTable[]=
                {{"Hajeb7Svc",ServiceMain},{NULL,NULL}};
    StartServiceCtrlDispatcher(DispatchTable);
  }
  return 0;
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
  m_ServiceStatus.dwServiceType = SERVICE_WIN32;
  m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
  m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
  m_ServiceStatus.dwWin32ExitCode = 0;
  m_ServiceStatus.dwServiceSpecificExitCode = 0;
  m_ServiceStatus.dwCheckPoint = 0;
  m_ServiceStatus.dwWaitHint = 0;

  m_ServiceStatusHandle = RegisterServiceCtrlHandler("Hajeb7Svc", 
                                            ServiceCtrlHandler); 
  if (m_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
  {
    return;
  }
  m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
  m_ServiceStatus.dwCheckPoint = 0;
  m_ServiceStatus.dwWaitHint = 0;
  if (!SetServiceStatus (m_ServiceStatusHandle, &m_ServiceStatus))
  {
  }

  	/********************************************************************************************/
	HANDLE hMapFile;

	SECURITY_ATTRIBUTES security;
	ZeroMemory(&security, sizeof(security));
	security.nLength = sizeof(security);
	ConvertStringSecurityDescriptorToSecurityDescriptor("D:P(A;OICI;GA;;;SY)(A;OICI;GA;;;BA)(A;OICI;GA;;;IU)",SDDL_REVISION_1,&security.lpSecurityDescriptor,NULL);

	hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,&security,PAGE_READWRITE,0,BUFF_SIZE,szName);
	if (hMapFile == NULL) 
	{ 
		printf(TEXT("Could not create file mapping object (%d).\n"), GetLastError());
		return ;
	}

   
	MySharedData* pSharedData = (MySharedData *) MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,BUFF_SIZE);           
 

	pSharedData->flag = SIG_NO_DATA;		// reset

	HANDLE ghMutex = CreateMutex(&security,FALSE,TEXT("Global\\MyMutexObject"));
	if (ghMutex == NULL) 
    {
        printf("CreateMutex error: %d\n", GetLastError());
        return ;
    }

	LocalFree(security.lpSecurityDescriptor);

	/********************************************************************************************/

  bRunning=true;
  while(bRunning)
  {
    Sleep(3000);
    //Place Your Code for processing here....
  }

	UnmapViewOfFile(pSharedData);
	CloseHandle(ghMutex);
	CloseHandle(hMapFile);
  return;
}

void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
  switch(Opcode)
  {
////////////////////////////////////////////////////////////////////////////////////
//Added By Jaisvar on 04/11/07 to recieve a custom message from a user app

	case SERVICE_CONTROL_CUSTOM_MESSAGE:
		LaunchAppIntoDifferentSession(); 
	break; 
	case SERVICE_CONTROL_PAUSE: 
      m_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
      break;
    case SERVICE_CONTROL_CONTINUE:
      m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
      break;
    case SERVICE_CONTROL_STOP:
      m_ServiceStatus.dwWin32ExitCode = 0;
      m_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
      m_ServiceStatus.dwCheckPoint = 0;
      m_ServiceStatus.dwWaitHint = 0;

      SetServiceStatus (m_ServiceStatusHandle,&m_ServiceStatus);
      bRunning=false;
      break;
    case SERVICE_CONTROL_INTERROGATE:
      break; 
  }
  return;
}

BOOL InstallService()
{
  char strDir[1024];
  SC_HANDLE schSCManager,schService;
  GetCurrentDirectory(1024,strDir);
  strcat(strDir,"\\Service1.exe"); 
  schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

  if (schSCManager == NULL) 
    return false;
  LPCTSTR lpszBinaryPathName=strDir;

  schService = CreateService(schSCManager,"Hajeb7Svc", 
        "Hajeb7", // service name to display
     SERVICE_ALL_ACCESS, // desired access 
     SERVICE_WIN32_OWN_PROCESS, // service type 
     SERVICE_AUTO_START, // start type 
     SERVICE_ERROR_NORMAL, // error control type 
     lpszBinaryPathName, // service's binary 
     NULL, // no load ordering group 
     NULL, // no tag identifier 
     NULL, // no dependencies
     NULL, // LocalSystem account
     NULL); // no password

  if (schService == NULL)
    return false; 

  ::CloseServiceHandle(schService);
  return true;
}

BOOL DeleteService()
{
  SC_HANDLE schSCManager;
  SC_HANDLE hService;
  schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

  if (schSCManager == NULL)
    return false;
  hService=OpenService(schSCManager,"Hajeb7Svc",SERVICE_ALL_ACCESS);
  if (hService == NULL)
    return false;
  if(DeleteService(hService)==0)
    return false;
  if(CloseServiceHandle(hService)==0)
    return false;

return true;
}

////////////////////////////////////////////////////////////////////////////////////
//Added By Jaisvar on 04/11/07 to Launch the process into a different session

/////////////////////////////////////////////////////////////////////////  
BOOL LaunchAppIntoDifferentSession() 
{
   PROCESS_INFORMATION pi;
   STARTUPINFO si;
   BOOL bResult = FALSE;
   DWORD dwSessionId,winlogonPid;
   HANDLE hUserToken,hUserTokenDup,hPToken,hProcess;
   DWORD dwCreationFlags;

// Log the client on to the local computer.

   dwSessionId = WTSGetActiveConsoleSessionId();

//////////////////////////////////////////
   // Find the winlogon process
////////////////////////////////////////

   PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return 1 ;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry))
	{
		return 1 ;
	}

	do
	{
		if (_stricmp(procEntry.szExeFile, "winlogon.exe") == 0)
		{
			// We found a winlogon process...make sure it's running in the console session
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) && winlogonSessId == dwSessionId)
			{
				winlogonPid = procEntry.th32ProcessID;
				break;
			}
		}

	} while (Process32Next(hSnap, &procEntry));

////////////////////////////////////////////////////////////////////////

   WTSQueryUserToken(dwSessionId,&hUserToken);
   dwCreationFlags = NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE;
   ZeroMemory(&si, sizeof(STARTUPINFO));
   si.cb= sizeof(STARTUPINFO);
   si.lpDesktop = "winsta0\\default";
   ZeroMemory(&pi, sizeof(pi));
   TOKEN_PRIVILEGES tp;
   LUID luid;
   hProcess = OpenProcess(MAXIMUM_ALLOWED,FALSE,winlogonPid);

   if(!::OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY
									|TOKEN_DUPLICATE|TOKEN_ASSIGN_PRIMARY|TOKEN_ADJUST_SESSIONID
									|TOKEN_READ|TOKEN_WRITE,&hPToken))
   {
			   int abcd = GetLastError();
			   printf("Process token open Error: %u\n",GetLastError()); 
   }

   if (!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&luid))
   {
	   printf("Lookup Privilege value Error: %u\n",GetLastError());
   }
   tp.PrivilegeCount =1;
   tp.Privileges[0].Luid =luid;
   tp.Privileges[0].Attributes =SE_PRIVILEGE_ENABLED;

   DuplicateTokenEx(hPToken,MAXIMUM_ALLOWED,NULL,SecurityIdentification,TokenPrimary,&hUserTokenDup);
   int dup = GetLastError();

   //Adjust Token privilege
   SetTokenInformation(hUserTokenDup,TokenSessionId,(void*)dwSessionId,sizeof(DWORD));

   if (!AdjustTokenPrivileges(hUserTokenDup,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),(PTOKEN_PRIVILEGES)NULL,NULL))
   {
	   int abc =GetLastError();
	   printf("Adjust Privilege value Error: %u\n",GetLastError());
   }

   if (GetLastError()== ERROR_NOT_ALL_ASSIGNED)
   {
	 printf("Token does not have the provilege\n");
   }

   LPVOID pEnv =NULL;

   if(CreateEnvironmentBlock(&pEnv,hUserTokenDup,TRUE))
   {
	   dwCreationFlags|=CREATE_UNICODE_ENVIRONMENT;
   }
   else
	  pEnv=NULL;

// Launch the process in the client's logon session.

  bResult = CreateProcessAsUser(
      hUserTokenDup,            // client's access token
      _T(keyMonitorPath.c_str()),              // file to execute
      NULL,     // command line
      NULL,              // pointer to process SECURITY_ATTRIBUTES
      NULL,              // pointer to thread SECURITY_ATTRIBUTES
      FALSE,             // handles are not inheritable
      dwCreationFlags,  // creation flags
      pEnv,              // pointer to new environment block 
      NULL,              // name of current directory 
      &si,               // pointer to STARTUPINFO structure
      &pi                // receives information about new process
   );

   bResult = CreateProcessAsUser(
      hUserTokenDup,            // client's access token
	  _T(processMonitorPath.c_str()),              // file to execute
      NULL,     // command line
      NULL,              // pointer to process SECURITY_ATTRIBUTES
      NULL,              // pointer to thread SECURITY_ATTRIBUTES
      FALSE,             // handles are not inheritable
      dwCreationFlags,  // creation flags
      pEnv,              // pointer to new environment block 
      NULL,              // name of current directory 
      &si,               // pointer to STARTUPINFO structure
      &pi                // receives information about new process
   );


	bResult = CreateProcessAsUser(
      hUserTokenDup,            // client's access token
	  _T(screenshotMonitorPath.c_str()),              // file to execute
      NULL,     // command line
      NULL,              // pointer to process SECURITY_ATTRIBUTES
      NULL,              // pointer to thread SECURITY_ATTRIBUTES
      FALSE,             // handles are not inheritable
      dwCreationFlags,  // creation flags
      pEnv,              // pointer to new environment block 
      NULL,              // name of current directory 
      &si,               // pointer to STARTUPINFO structure
      &pi                // receives information about new process
   );


	bResult = CreateProcessAsUser(
      hUserTokenDup,            // client's access token
	  _T(netMonitorPath.c_str()),              // file to execute
      NULL,     // command line
      NULL,              // pointer to process SECURITY_ATTRIBUTES
      NULL,              // pointer to thread SECURITY_ATTRIBUTES
      FALSE,             // handles are not inheritable
      dwCreationFlags,  // creation flags
      pEnv,              // pointer to new environment block 
      NULL,              // name of current directory 
      &si,               // pointer to STARTUPINFO structure
      &pi                // receives information about new process
   );

	bResult = CreateProcessAsUser(
      hUserTokenDup,            // client's access token
	  _T(serverPath.c_str()),              // file to execute
      NULL,     // command line
      NULL,              // pointer to process SECURITY_ATTRIBUTES
      NULL,              // pointer to thread SECURITY_ATTRIBUTES
      FALSE,             // handles are not inheritable
      dwCreationFlags,  // creation flags
      pEnv,              // pointer to new environment block 
      NULL,              // name of current directory 
      &si,               // pointer to STARTUPINFO structure
      &pi                // receives information about new process
   );
 

// End impersonation of client.

//GetLastError Shud be 0

   int iResultOfCreateProcessAsUser = GetLastError();

//Perform All the Close Handles task

  CloseHandle(hProcess);
  CloseHandle(hUserToken);
  CloseHandle(hUserTokenDup);
  CloseHandle(hPToken);
	
 return 0;
}