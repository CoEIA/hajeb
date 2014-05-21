// CustomMessageSender.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define SERVICE_NAME _T("Hajeb7Svc")
//CUSTOM MESSAGE FOR SERVICE TO LAUNCH AN APP INTO SESSION1
#define SERVICE_CONTROL_CUSTOM_MESSAGE 0x0096

int _tmain(int argc, _TCHAR* argv[])
{
	SC_HANDLE hMyService,hSCM;
	BOOL bSuccess;
	SERVICE_STATUS status;
    hSCM = OpenSCManager(0,0,SC_MANAGER_CONNECT);
	if(!hSCM)
	{
		printf("Open SCM failed with error %u",GetLastError());
	}
	hMyService = OpenService(hSCM,SERVICE_NAME,SERVICE_USER_DEFINED_CONTROL);
	if(!hMyService)
	{
		printf("Open SCM failed with error %u",GetLastError());
	}
	bSuccess = ControlService(hMyService,SERVICE_CONTROL_CUSTOM_MESSAGE,&status);
	if(!bSuccess)
	{
		printf("Control Service failed with error %u",GetLastError());
	}
	CloseServiceHandle(hMyService);
	CloseServiceHandle(hSCM);
	return 0;
}

