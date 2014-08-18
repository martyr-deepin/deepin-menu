#-------------------------------------------------
#
# Project created by QtCreator 2014-08-14T14:55:09
#
#-------------------------------------------------

QT       += core gui x11extras dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = deepin-menu
TEMPLATE = app


SOURCES += main.cpp \
    dmenubase.cpp \
    ddesktopmenu.cpp \
    utils.cpp \
    dmenucontent.cpp \
    dbus_manager_adaptor.cpp \
    dbus_menu_adaptor.cpp \
    manager_object.cpp \
    menu_object.cpp \
    ddockmenu.cpp

HEADERS  += \
    dmenubase.h \
    ddesktopmenu.h \
    utils.h \
    dmenucontent.h \
    dbus_manager_adaptor.h \
    dbus_menu_adaptor.h \
    manager_object.h \
    menu_object.h \
    ddockmenu.h

LIBS += -lxcb
