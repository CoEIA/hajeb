#-------------------------------------------------
#
# Project created by QtCreator 2010-12-28T08:08:56
#
#-------------------------------------------------

QT       += core gui sql network

TARGET = Hajeb
TEMPLATE = app

#CONFIG +=   console

SOURCES += main.cpp\
    mainwindow.cpp \
    websitelockerwidget.cpp \
    programlockerwidget.cpp \
    logswidget.cpp \
    screenshotwidget.cpp \
    settingswidget.cpp \
    browsetimewidget.cpp \
    wizarddialog.cpp \
    logindialog.cpp \
    resettingregistry.cpp \
    hajebservices.cpp \
    useraccounts.cpp \
    database.cpp \
    homewidget.cpp \
    SlidingStackedWidget.cpp \
    license.cpp \
    server.cpp \
    applicationstatus.cpp \
    ../Global/lunhalgorithm.cpp \
    ../Global/keygenerator.cpp \
    ../Global/keyvalidator.cpp \
    serverhandler.cpp \
    useraccountsdialog.cpp \
    updatehandler.cpp \
    luncher.cpp \
    utility.cpp \
    treemodel.cpp \
    item.cpp \
    action.cpp \
    actionqueue.cpp \
    actionqueueoptimizer.cpp \
    listimporter.cpp \
    listupdater.cpp \
    networkmanager.cpp \
    xmllistreader.cpp \
    xmllistwriter.cpp \
    listtable.cpp \
    registryreader.cpp \
    registryentry.cpp \
    addprogramdialog.cpp \
    programmodel.cpp

HEADERS  += mainwindow.h \
    websitelockerwidget.h \
    programlockerwidget.h \
    logswidget.h \
    screenshotwidget.h \
    settingswidget.h \
    browsetimewidget.h \
    version.h \
    wizarddialog.h \
    logindialog.h \
    resettingregistry.h \
    hajebservices.h \
    useraccounts.h \
    database.h \
    homewidget.h \
    SlidingStackedWidget.h \
    license.h \
    server.h \
    applicationstatus.h \
    ../Global/keygenerator.h \
    ../Global/keyvalidator.h \
    ../Global/lunhalgorithm.h \
    ../Global/global.h \
    serverhandler.h \
    constant.h \
    useraccountsdialog.h \
    updatehandler.h \
    luncher.h \
    utility.h \
    treemodel.h \
    item.h \
    action.h \
    actionqueue.h \
    actionqueueoptimizer.h \
    listimporter.h \
    listupdater.h \
    networkmanager.h \
    xmllistreader.h \
    xmllistwriter.h \
    xmlconstants.h \
    listtable.h \
    websitelocker.h \
    registryreader.h \
    registryobject.h \
    registryentry.h \
    addprogramdialog.h \
    programmodel.h

RESOURCES += \
    CoEIAHajeb.qrc

OTHER_FILES += \
    CoEIAHajeb.rc \
    CoEIAHajeb.qss

win32:RC_FILE = CoEIAHajeb.rc

FORMS += \
    websitelockerwidget.ui \
    programlockerwidget.ui \
    logswidget.ui \
    screenshotwidget.ui \
    settingswidget.ui \
    browsetimewidget.ui \
    logindialog.ui \
    homewidget.ui \
    addprogramdialog.ui

win32: LIBS += -lnetapi32 -lws2_32

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




































