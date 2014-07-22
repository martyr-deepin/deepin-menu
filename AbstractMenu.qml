import QtQuick 2.2
import QtQuick.Window 2.0

Item {
    id: abstract_menu
    width: childrenRect.width
    height: childrenRect.height

    property QtObject parentMenu: null
    property QtObject childMenu: null
    property int glowRadius: 0

    default property alias contnt: content.children

    property int originX
    property int originY
    onWidthChanged: {
        if (originX + width - glowRadius > global_screen.desktopAvailableWidth) {
            x = parentMenu ? parentMenu.x - width + 2 * glowRadius
                           : global_screen.desktopAvailableWidth - width + glowRadius
        }
    }
    onHeightChanged: {
        if (originY + height - glowRadius > global_screen.desktopAvailableHeight) {
            y = global_screen.desktopAvailableHeight - height + glowRadius
        }
    }

    function startDestroy() { destroy_timer.restart() }
    function stopDestroy() { destroy_timer.stop() }

    function destroySubMenu() {
        if (childMenu) {
            childMenu.destroySubMenu()
            childMenu.destroy()
        }
    }

    function destroySubMenuLater() {
        if (childMenu) {
            childMenu.destroySubMenu()
            childMenu.startDestroy()
        }
    }

    Timer {
        id: destroy_timer
        interval: 100
        onTriggered: abstract_menu.destroySubMenu()
    }

    Item {
        id: content
        width: childrenRect.width
        height: childrenRect.height
    }
}
