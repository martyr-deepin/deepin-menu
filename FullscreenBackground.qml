import QtQuick 2.0
import QtQuick.Window 2.0

Item {
    id: global_screen
    width: Screen.width
    height: Screen.height

    property int desktopAvailableWidth: Screen.desktopAvailableWidth
    property int desktopAvailableHeight: Screen.desktopAvailableHeight

    function showMenu(menuJsonContent) {
        var content = JSON.parse(menuJsonContent);

        desktop_menu.originX = content.x - desktop_menu.glowRadius
        desktop_menu.originY = content.y - desktop_menu.glowRadius
        desktop_menu.x = desktop_menu.originX
        desktop_menu.y = desktop_menu.originY

        desktop_menu.setContent(JSON.parse(content.menuJsonContent))
    }

    MouseArea {
        anchors.fill: parent
        onClicked: Qt.quit()
    }

    DesktopMenu {
        id: desktop_menu
    }
}
