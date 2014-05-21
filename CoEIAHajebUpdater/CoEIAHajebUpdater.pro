#-------------------------------------------------
#
# Project created by QtCreator 2011-04-10T13:26:42
#
#-------------------------------------------------

QT       += core gui

TARGET = CoEIAHajebUpdater
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
    updatedialog.cpp

FORMS += \
    updatedialog.ui

HEADERS += \
    updatedialog.h \
    ../Global/global.h
