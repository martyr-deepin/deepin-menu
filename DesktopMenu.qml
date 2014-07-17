import QtQuick 2.2

Rectangle {
    id: global_menu
    width: content.width
    height: content.height

    property QtObject parentMenu: null
    property QtObject childMenu: null

    function startDestroy() { destroy_timer.restart() }
    function stopDestroy() { destroy_timer.stop() }

    function showSubMenu(x, y, content) {
        childMenu = Qt.createQmlObject("import QtQuick 2.1; DesktopMenu{x: %1; y: %2}".arg(x).arg(y),
                                       global_screen, "menu1")
        childMenu.parentMenu = global_menu
    }

    function destroySubMenu() {
        if (childMenu) {
            childMenu.destroySubMenu()
            childMenu.startDestroy()
        }
    }

    Timer {
        id: destroy_timer
        interval: 100
        onTriggered: global_menu.destroy()
    }

    MenuContent {
        id: content
    }
}
