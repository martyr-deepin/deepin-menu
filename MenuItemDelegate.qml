import QtQuick 2.0

import "MenuItem.js" as MenuItemJs

Component {
    Rectangle {
        id: itemArea
        width: ListView.view.width
        height: componentText.text == "" ? verticalPadding * 2 : Math.max(componentImage.implicitHeight 
																		  + verticalPadding * 2, 
																		  componentText.implicitHeight 
																		  + verticalPadding * 2)
        color: Qt.rgba(0, 0, 0, 0)
		
		property var fullscreenBg: ListView.view.fullscreenBg
		property int verticalPadding: ListView.view.verticalPadding
		property int horizontalPadding: ListView.view.horizontalPadding
		property int textLeftMargin: ListView.view.textLeftMargin
		property int textSize: ListView.view.textSize
		property color textColor: ListView.view.textColor
        property string componentSubMenu: itemSubMenu
		property string componentId: itemId
		
        Image {
            id: componentImage
            visible: itemIcon != ""
            source: itemIcon
            anchors.left: parent.left
			anchors.leftMargin: horizontalPadding
			anchors.rightMargin: horizontalPadding			
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: componentText
            visible: itemText != ""
            text: itemText
			color: textColor
            font.pixelSize: 12
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: parent.textLeftMargin
        }

        Rectangle {
			/* visible: itemArea.ListView.view.model.count == 0 */
			id: itemSeparator
			visible: componentText.text == ""
			width: 1
			height: itemArea.ListView.view.width 
			transformOrigin: Item.Center
			rotation: 90
			anchors.centerIn: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: Qt.rgba(0, 0, 0, 0) }
                GradientStop { position: 0.5; color: Qt.rgba(1, 1, 1, 0.5) }
                GradientStop { position: 1.0; color: Qt.rgba(0, 0, 0, 0) }
            }
        }

        MouseArea {
            id: mouseArea
            visible: !itemSeparator.visible
            anchors.fill: parent
            hoverEnabled: true

            onPressed: MenuItemJs.onPressed(parent, menu)
            onReleased: MenuItemJs.onReleased(parent, menu)
            onEntered: MenuItemJs.onEntered(parent, menu, fullscreenBg)
            onExited: MenuItemJs.onExited(parent, menu)
        }
    }
}
