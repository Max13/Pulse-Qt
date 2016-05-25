#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T18:40:32
#
#-------------------------------------------------

QT          =  core# network

#CONFIG      -=  app_bundle
CONFIG      +=  console c++11
TARGET      =   Pulse
TEMPLATE    =   app

include(../commons/commons.pri)
include(../../libs/qtservice/src/src/qtservice.pri)

SOURCES     +=  main.cpp \
                Launcher.cpp

HEADERS     +=  Launcher.hpp

DEFINES     +=  CONSOLE
