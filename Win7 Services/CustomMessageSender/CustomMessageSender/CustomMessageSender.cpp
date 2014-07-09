// CustomMessageSender.cpp : Defines the entry point for the console application.

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

