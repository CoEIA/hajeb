######################################################################
# Automatically generated by qmake (2.01a) Wed Apr 6 14:11:55 2011
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

Release:DESTDIR = release
Debug:DESTDIR = debug

Release:OBJECT_DIR = release/.obj
Debug:OBJECT_DIR = debug/.obj

Release:MOC_DIR = release/.moc
Debug:MOC_DIR = debug/.moc

Release:RCC_DIR = release/.rcc
Debug:RCC_DIR = debug/.rcc

Release:UI_DIR = release/.ui
Debug:UI_DIR = debug/.ui

# Input
HEADERS += ../Global/keygenerator.h keysgeneratorwidget.h ../Global/keyvalidator.h ../Global/lunhalgorithm.h
FORMS += keysgeneratorwidget.ui
SOURCES += keysgeneratorwidget.cpp ../Global/lunhalgorithm.cpp main.cpp \
    ../Global/keygenerator.cpp \
    ../Global/keyvalidator.cpp

QT += sql

OTHER_FILES += \
    db.sql
