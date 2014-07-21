TEMPLATE = app

QT += qml quick dbus

SOURCES += main.cpp \
    dbus_manager_adaptor.cpp \
    dbus_menu_adaptor.cpp \
    manager_object.cpp \
    menu_object.cpp \
    utils.cpp

RESOURCES += qml.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += dbus_manager_adaptor.h \
    dbus_menu_adaptor.h \
    manager_object.h \
    menu_object.h \
    utils.h

LIBS += -lxcb
