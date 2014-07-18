import QtQuick 2.2
import QtQuick.Window 2.0

Rectangle {
    id: global_menu
    width: content.width
    height: content.height

    property QtObject parentMenu: null
    property QtObject childMenu: null

    onWidthChanged: {
        if (x + width > global_screen.desktopAvailableWidth) {
            x = parentMenu.x - width
        }
    }

    function startDestroy() { destroy_timer.restart() }
    function stopDestroy() { destroy_timer.stop() }

    function showSubMenu(x, y, content) {
        childMenu = Qt.createQmlObject("import QtQuick 2.1; DesktopMenu{x: %1; y: %2}".arg(x).arg(y),
                                       global_screen, "menu1")
        childMenu.setContent(content)
        childMenu.parentMenu = global_menu
    }

    function destroySubMenu() {
        if (childMenu) {
            childMenu.destroySubMenu()
            childMenu.startDestroy()
        }
    }

    function requestFocus() { content.focus = true }

    function setContent(_content) { content.setContent(_content) }

    Timer {
        id: destroy_timer
        interval: 100
        onTriggered: global_menu.destroy()
    }

    MenuContent {
        id: content
    }
}
