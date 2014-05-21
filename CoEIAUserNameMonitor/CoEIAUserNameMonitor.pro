#-------------------------------------------------
#
# Project created by QtCreator 2011-01-06T11:28:11
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = CoEIAUserNameMonitor
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

SOURCES += main.cpp
