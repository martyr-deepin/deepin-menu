#-------------------------------------------------
#
# Project created by QtCreator 2014-08-14T14:55:09
#
#-------------------------------------------------

QT       += core gui x11extras dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = deepin-menu
TEMPLATE = app

CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkbase dtkwidget dtkutil dframeworkdbus

SOURCES += src/main.cpp \
    src/ddesktopmenu.cpp \
    src/utils.cpp \
    src/dmenucontent.cpp \
    src/dbus_manager_adaptor.cpp \
    src/dbus_menu_adaptor.cpp \
    src/manager_object.cpp \
    src/menu_object.cpp \
    src/ddockmenu.cpp \
    src/dmenuapplication.cpp \
    src/dabstractmenu.cpp

HEADERS  += \
    src/ddesktopmenu.h \
    src/utils.h \
    src/dmenucontent.h \
    src/dbus_manager_adaptor.h \
    src/dbus_menu_adaptor.h \
    src/manager_object.h \
    src/menu_object.h \
    src/ddockmenu.h \
    src/dmenuapplication.h \
    src/dabstractmenu.h

LIBS += -lxcb -lX11

RESOURCES += \
    images.qrc

target.path = /usr/bin
INSTALLS += target
