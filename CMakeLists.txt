cmake_minimum_required(VERSION 3.6)
project(deepin_menu)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
add_compile_options(-fPIC)


find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(QT4 REQUIRED)
ADD_DEFINITIONS(${QT_DEFINITIONS})

include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# include qt
include_directories(/usr/include/x86_64-linux-gnu/qt5)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtCore)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtDBus)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtWidgets)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtGui)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtX11Extras)

# include dtk
include_directories(/usr/include/libdtk-2.0/DCore)
include_directories(/usr/include/libdtk-2.0/DWidget)

# include dbus
include_directories(/usr/include/libdframeworkdbus-1.0)

link_directories(/usr/lib/x86_64-linux-gnu/)

set(deepin_menu_libs
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5DBus
        Qt5X11Extras

        dframeworkdbus
        dtkcore
        dtkwidget
        xcb
        X11
        )

set(deepinMenu_HEADERS
        src/ddesktopmenu.h
        src/utils.h
        src/dmenucontent.h
        src/dbus_manager_adaptor.h
        src/dbus_menu_adaptor.h
        src/manager_object.h
        src/menu_object.h
        src/ddockmenu.h
        src/dmenuapplication.h
        src/dabstractmenu.h
        )
SET(deepinMenu_QRCS images.qrc)

SET(deepinMenu_SRCS
        src/main.cpp
        src/ddesktopmenu.cpp
        src/utils.cpp
        src/dmenucontent.cpp
        src/dbus_manager_adaptor.cpp
        src/dbus_menu_adaptor.cpp
        src/manager_object.cpp
        src/menu_object.cpp
        src/ddockmenu.cpp
        src/dmenuapplication.cpp
        src/dabstractmenu.cpp
        ${deepinMenu_QRCS}
        )

ADD_EXECUTABLE(deepin-menu
        ${deepinMenu_SRCS}
        ${deepinMenu_HEADERS_MOC}
        ${deepinMenu_UIS_UIC}
        ${deepinMenu_QRC_RCC}
        )

TARGET_LINK_LIBRARIES(deepin-menu ${deepin_menu_libs})

