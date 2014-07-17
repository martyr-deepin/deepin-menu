import QtQuick 2.0
import QtQuick.Window 2.0

Item {
    id: global_screen
    width: Screen.width
    height: Screen.height

    function setPosition(x, y) {
        desktop_menu.x = x
        desktop_menu.y = y
    }

    MouseArea {
        anchors.fill: parent
        onClicked: Qt.quit()
    }

    DesktopMenu {
        id: desktop_menu
    }
}
