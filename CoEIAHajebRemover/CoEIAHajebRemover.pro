
TARGET = HajebRemover
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

HEADERS += \
    removerwidget.h \
    resettingregistry.h

SOURCES += \
    removerwidget.cpp \
    main.cpp

FORMS += \
    removerwidget.ui

OTHER_FILES += \
    CoEIAHajebRemover.rc

win32:RC_FILE = CoEIAHajebRemover.rc

RESOURCES += \
    CoEIAHajebRemover.qrc

