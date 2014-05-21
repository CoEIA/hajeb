#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QChar>
#include <QSemaphore>

#define BUFFER_SIZE 2000

extern QString _previousAppName ;
extern QString _appName ;
extern bool _addRecord;
extern QString keyLogBuffer;

extern QChar _buffer[BUFFER_SIZE];
extern QSemaphore emptyCount;
extern QSemaphore fullCount;
extern int bufferIndex;
extern int size;


#endif // GLOBALDATA_H
