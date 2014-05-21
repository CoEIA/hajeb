#-------------------------------------------------
#
# Project created by QtCreator 2011-01-17T10:47:03
#
#-------------------------------------------------

QT       += core gui sql

TARGET = CoEIANetMonitor
TEMPLATE = app


Release:DESTDIR = ../build/bin
Debug:DESTDIR = debug

Release:OBJECT_DIR = release/.obj
Debug:OBJECT_DIR = debug/.obj

Release:MOC_DIR = release/.moc
Debug:MOC_DIR = debug/.moc

Release:RCC_DIR = release/.rcc
Debug:RCC_DIR = debug/.rcc

Release:UI_DIR = release/.ui
Debug:UI_DIR = debug/.ui


SOURCES += main.cpp\
        widget.cpp \
    countertimer.cpp

HEADERS  += widget.h \
    countertimer.h

RESOURCES += \
    CoEIANetMonitor.qrc

FORMS += \
    widget.ui

OTHER_FILES +=
