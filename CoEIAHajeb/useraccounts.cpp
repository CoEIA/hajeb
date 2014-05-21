#include <QProcess>
#include <QDebug>
#include <windows.h>
#include <lm.h>
#include <assert.h>
#include <stdio.h>
#include "useraccounts.h"

QStringList getAllAdminsName() {
    QStringList userName ;
    QProcess* process = new QProcess;
    process->start("net localgroup administrators");
    process->waitForStarted();
    process->waitForFinished();
    QString output = process->readAll();
    QStringList lines = output.split("\n");
    int i = 0;
    QString line = lines.at(i);
    while (!line.contains("-----------------"))
        line = lines.at(++i);
    ++i;
    while (i < lines.size()-3)
        userName << lines.at(i++).trimmed();
    return userName;
}

QString getCurrentUserName() {
    TCHAR buffer[100];
    DWORD buffer_size = 100;
    GetUserName(buffer,&buffer_size);
    return QString::fromWCharArray(buffer);
}

QStringList getAllUserAccounts() {
    QStringList allUsers;
    LPUSER_INFO_0 pBuf = NULL;
    LPUSER_INFO_0 pTmpBuf;
    DWORD dwLevel = 0;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    DWORD dwResumeHandle = 0;
    DWORD i;
    DWORD dwTotalCount = 0;
    NET_API_STATUS nStatus;
    LPTSTR pszServerName = NULL;
    do {
        nStatus = NetUserEnum((LPCWSTR) pszServerName,dwLevel,FILTER_NORMAL_ACCOUNT,(LPBYTE*)&pBuf,dwPrefMaxLen,&dwEntriesRead,&dwTotalEntries,&dwResumeHandle);
       if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
           if ((pTmpBuf = pBuf) != NULL) {
               for (i = 0; (i < dwEntriesRead); i++) {
                   assert(pTmpBuf != NULL);
                   if (pTmpBuf == NULL) {
                       fprintf(stderr, "An access violation has occurred\n");
                       break;
                   }

                   allUsers << QString::fromWCharArray(pTmpBuf->usri0_name);
                   pTmpBuf++;
                   dwTotalCount++;
               }
           }
       } else
           qDebug() << "A system error has occurred:" <<  nStatus ;

       if (pBuf != NULL) {
          NetApiBufferFree(pBuf);
          pBuf = NULL;
       }
    } while (nStatus == ERROR_MORE_DATA);

    if (pBuf != NULL)
       NetApiBufferFree(pBuf);

    return allUsers;
}
