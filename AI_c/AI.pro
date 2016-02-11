QT += core
QT -= gui

CONFIG += c++11

TARGET = AI
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    exceptionAI.cpp \
    mathfunctions.cpp \
    utilities.cpp \
    point.cpp \
    fieldobject.cpp \
    robot.cpp \
    gamestatus.cpp

HEADERS += \
    exceptionAI.h \
    mathfunctions.h \
    utilities.h \
    point.h \
    fieldobject.h \
    robot.h \
    gamestatus.h
