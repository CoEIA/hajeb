// CoEIAUserNameMonitor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Windows.h"
#include "stdafx.h"
#include "Winsvc.h"
#include "time.h"
#include <WtsApi32.h>
#include <UserEnv.h>
#include <tlhelp32.h>

#include <iostream>
#include <string>

#pragma comment(lib,"WtsApi32.lib")
#pragma comment(lib,"UserEnv.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwSessionId,dwExplorerLogonPid,dwSize,dwRegDataSize;
    HANDLE hProcess,hPToken;
    char szUserName[MAX_PATH];
    char szRegData[MAX_PATH];
    char szRegPath[500] = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    HKEY hKey; //Handle to registry Key
    
    long lRegResult; //Registry operation result
    
    
    //Get the active desktop session id
    
    dwSessionId = WTSGetActiveConsoleSessionId();
    
    //We find the explorer process since it will have the user token
    
    
    //////////////////////////////////////////
    
    // Find the explorer process
    
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
        if (_stricmp(procEntry.szExeFile, "explorer.exe") == 0)
        {
          DWORD dwExplorerSessId = 0;
          if (ProcessIdToSessionId(procEntry.th32ProcessID, &dwExplorerSessId) && dwExplorerSessId == dwSessionId)
            {
                dwExplorerLogonPid = procEntry.th32ProcessID;
                break;
            }
        }

    } while (Process32Next(hSnap, &procEntry));
    
    ////////////////////////////////////////////////////////////////////////
    
    hProcess = OpenProcess(MAXIMUM_ALLOWED,FALSE,dwExplorerLogonPid);
    
    if(!::OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY|TOKEN_DUPLICATE|TOKEN_ASSIGN_PRIMARY|TOKEN_ADJUST_SESSIONID|TOKEN_READ|TOKEN_WRITE,&hPToken))
    {
        int abcd = GetLastError();
        printf("Process token open Error: %u\n",GetLastError());
    }
       
   
   //Impersonate the explorer token which runs under the user account
   
   ImpersonateLoggedOnUser(hPToken);
   
   int iImpersonateResult = GetLastError();
   
   if(iImpersonateResult == ERROR_SUCCESS)
   {
     //GetUserName will now return the username
   
     GetUserName(szUserName,&dwSize);


    //Since the thread is running as the user we can access the HKCU now
   
     dwRegDataSize = sizeof(szRegData);
     lRegResult = RegOpenKeyEx(HKEY_CURRENT_USER,
               szRegPath,0,KEY_QUERY_VALUE,&hKey);
     if (lRegResult == ERROR_SUCCESS)
        RegQueryValueEx(hKey,_T("SideBar"),NULL,NULL,
               (LPBYTE)&szRegData,&dwRegDataSize);
   }
   //Once the operation is over revert back to system account.
   
   RevertToSelf();

   printf("%s\n",szUserName);
	return 0;
}

