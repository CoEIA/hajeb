CoEIAUserNameMonitor is a user process that log current user name in a txt file.

It help other services to obtain a currently logged user name because services cannot get a currently logged user name it just get a process owner and it will be SYSTEM process.

After it running , a service can read the txt file and get the user name so serivce can do there work regard the currently user name.

This process must run automatically when the user log-in windows. the trick is to add it to list of automatically started process by add the following key to the windows registry.

HKEY_LOCAL_MACHINE/SOFTWARE/MICROSOFT/WINDOWS/CURRENT_VERSION/RUN

e.g:
KEYHOOK	REG_SZ	C:\KEYHOOK.EXE


OR 
HKEY_CURRENT_USER/SOFTWARE/M../W../CURRENT_VERSION/RUN


After it write the user name in the txt file it close automatically.