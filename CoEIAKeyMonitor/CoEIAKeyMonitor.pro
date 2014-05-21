#-------------------------------------------------
#
# Project created by QtCreator 2011-01-01T09:08:08
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = CoEIAKeyMonitor
CONFIG   -= app_bundle
#CONFIG += console
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
    consumerthread.cpp \
    consumer.cpp

LIBS += -luser32


HEADERS += \
    globaldata.h \
    consumerthread.h \
    consumer.h
