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
