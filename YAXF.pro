#-------------------------------------------------
#
# Project created by QtCreator 2018-06-24T17:24:58
#
#-------------------------------------------------

CONFIG  += debug
QT      += widgets

TARGET   = YAXF
TEMPLATE = app


HEADERS += \
        mainwindow.h \
        config.h \
        settings.h \
        settingsmanager.h

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        config.cpp \
        settingsmanager.cpp

FORMS += \
        mainwindow.ui \
        config.ui

RESOURCES += \
        icons.qrc

unix{
LIBS += -L/usr/local/lib -lusb-1.0
}
win32{
LIBS += -L/mingw64/bin -lusb-1.0
}
