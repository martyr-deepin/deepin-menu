import QtQuick 2.0

AbstractMenu {
    id: global_menu
    menuContent: content
    glowRadius: desktop_menu_background.glowRadius

    function showSubMenu(x, y, content) {
        var originX = x - glowRadius
        var originY = y - glowRadius

        childMenu = Qt.createQmlObject("import QtQuick 2.1; DesktopMenu{x: %1; y: %2}".arg(originX).arg(originY),
                                       global_screen, "menu1")
        childMenu.parentMenu = global_menu
        childMenu.originX = originX
        childMenu.originY = originY
        childMenu.setContent(content)
    }

    DesktopMenuBackground {
        id: desktop_menu_background
        MenuContent { id: content }
    }
}
