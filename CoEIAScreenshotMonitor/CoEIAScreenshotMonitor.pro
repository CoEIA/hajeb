#-------------------------------------------------
#
# Project created by QtCreator 2011-01-08T14:06:57
#
#-------------------------------------------------

QT       += core gui sql


TARGET = CoEIAScreenshotMonitor
#CONFIG   += console
CONFIG   -= app_bundle

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

SOURCES += main.cpp \
    screenshotmonitor.cpp

HEADERS += \
    screenshotmonitor.h

