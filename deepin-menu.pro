#-------------------------------------------------
#
# Project created by QtCreator 2014-08-14T14:55:09
#
#-------------------------------------------------

QT       += core gui x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = deepin-menu
TEMPLATE = app


SOURCES += main.cpp \
    dmenubase.cpp \
    ddesktopmenu.cpp \
    utils.cpp \
    dmenucontent.cpp

HEADERS  += \
    dmenubase.h \
    ddesktopmenu.h \
    utils.h \
    dmenucontent.h

LIBS += -lxcb
