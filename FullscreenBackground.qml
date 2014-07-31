import QtQuick 2.0
import QtQuick.Window 2.0
import DBus.Com.Deepin.Daemon.Display 1.0

Item {
    id: global_screen
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    property int desktopAvailableWidth: width
    property int desktopAvailableHeight: height

    Display { id: dbus_display }
    Monitor { id: dbus_monitor }

    function _calculateDesktopAvailableSize(x, y) {
        var monitors = dbus_display.monitors
        for(var i = 0; i < monitors.length; i++) {
            dbus_monitor.path = monitors[i]
            if(dbus_monitor.x < x
                    && dbus_monitor.y < y
                    && dbus_monitor.x + dbus_monitor.width > x
                    && dbus_monitor.y + dbus_monitor.height > y)
            {
                desktopAvailableWidth = dbus_monitor.x + dbus_monitor.width
                desktopAvailableHeight = dbus_monitor.y + dbus_monitor.height
            }
        }
    }

    function showMenu(menuJsonContent) {
        var content = JSON.parse(menuJsonContent)
        _calculateDesktopAvailableSize(content.x, content.y)
        content.isDockMenu ? _showDockMenu(content.x, content.y, JSON.parse(content.menuJsonContent))
                           : _showDesktopMenu(content.x, content.y, JSON.parse(content.menuJsonContent));
    }

    function _showDesktopMenu(x, y, content) {
        desktop_menu.originX = x - desktop_menu.glowRadius
        desktop_menu.originY = y - desktop_menu.glowRadius
        desktop_menu.x = desktop_menu.originX
        desktop_menu.y = desktop_menu.originY

        desktop_menu.setContent(content)
        desktop_menu.visible = true
    }

    function _showDockMenu(x, y, content) {
        dock_menu.setContent(content)

        dock_menu.originX = x - dock_menu.width / 2
        dock_menu.originY = y - dock_menu.height + dock_menu.glowRadius
        dock_menu.x = dock_menu.originX
        dock_menu.y = dock_menu.originY

        dock_menu.visible = true
    }

    function setChecked(id , checked) { desktop_menu.visible ? desktop_menu.setChecked(id, checked)
                                                             : dock_menu.setChecked(id, checked) }
    function setText(id, text) { desktop_menu.visible ? desktop_menu.setText(id, text)
                                                      : dock_menu.setText(id, text) }
    function setActive(id, active) { desktop_menu.visible ? desktop_menu.setActive(id, active)
                                                          : dock_menu.setActive(id, active) }

    MouseArea {
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill: parent
        onClicked: _utils_.menuDisappeared()
    }

    DesktopMenu {
        id: desktop_menu
        visible: false
    }

    DockMenu {
        id: dock_menu
        visible: false
    }
}
