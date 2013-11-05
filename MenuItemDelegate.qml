import QtQuick 2.0

import "MenuItem.js" as MenuItemJs

Component {
    id: menuItemDelegate

    Rectangle {
        id: itemArea
        /* width: ListView.width */
        width: menuItems.width
        height: 30
        /* color: mouseArea.pressed ? Qt.rgba(100, 0, 0, 1) : Qt.rgba(0, 100, 0, 1) */
		color: Qt.rgba(1, 1, 1, 0)

        Image {
            id: componentImage
            source: itemIcon
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin:2
        }

        Text {
            id: componentText
            text: itemText
            font.pixelSize: 12
            anchors.left: componentImage.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 2
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true

            onPressed: MenuItemJs.onPressed(parent)
            onReleased: MenuItemJs.onReleased(parent)
            onEntered: MenuItemJs.onEntered(parent)
            onExited: MenuItemJs.onExited(parent)
        }
    }
}
