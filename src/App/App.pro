#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T18:40:32
#
#-------------------------------------------------

QT          += core gui network svg widgets

CONFIG      -=  app_bundle
TARGET      =   Pulse-App
TEMPLATE    =   app

include(../commons/commons.pri)

SOURCES     +=  main.cpp \
                Controllers/MyController.cpp \
                Requester.cpp

HEADERS     +=  Controllers/MyController.hpp \
                Requester.hpp
