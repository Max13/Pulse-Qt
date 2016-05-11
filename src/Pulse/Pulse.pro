#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T18:40:32
#
#-------------------------------------------------

QT          += core gui network svg widgets

TARGET      = Pulse
TEMPLATE    = app

SOURCES     +=  main.cpp \
                Requester.cpp \
                MyService.cpp

HEADERS     +=  Requester.hpp \
                MyService.hpp

RESOURCES   +=  resources/resources.qrc

OTHER_FILES +=  resources/icon.icns \
                resources/icon.ico \
                resources/icon.svg \
                resources/Info.plist

include(../../libs/qtservice/src/src/qtservice.pri)

osx {
    ICON                =   resources/icon.icns
    QMAKE_INFO_PLIST    =   resources/Info.plist
}
win32:RC_ICONS          =   resources/icon.ico
