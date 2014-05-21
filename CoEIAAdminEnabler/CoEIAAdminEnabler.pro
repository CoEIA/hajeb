#-------------------------------------------------
#
# Project created by QtCreator 2011-03-22T07:48:17
#
#-------------------------------------------------

QT       += core gui

TARGET = CoEIAAdminManager
TEMPLATE = app


SOURCES += main.cpp\
        adminenablerwidget.cpp \
    passworddialog.cpp

HEADERS  += adminenablerwidget.h \
    passworddialog.h

FORMS    += adminenablerwidget.ui \
    passworddialog.ui

RESOURCES += \
    CoEIAAdminEnabler.qrc

OTHER_FILES += \
    CoEIAAdminEnabler.rc

win32:RC_FILE = CoEIAAdminEnabler.rc
