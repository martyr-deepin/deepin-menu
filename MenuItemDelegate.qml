import QtQuick 2.0

import "MenuItem.js" as MenuItemJs

Component {
    id: menuItemDelegate

    Rectangle {
        id: itemArea
        width: ListView.view.width
        height: componentText.text == "" ? menu.menuItemPadding * 2 : Math.max(componentImage.implicitHeight 
																			   + menu.menuItemPadding * 2, 
																			   componentText.implicitHeight 
																			   + menu.menuItemPadding * 2)
        color: Qt.rgba(0, 0, 0, 0)
		
        property string componentSubMenu: itemSubMenu
		property string componentId: itemId
		
		/* Component.onCompleted: { */
		/* 	console.log(componentImage.implicitHeight) */
		/* 	console.log(componentText.implicitHeight) */
		/* 	console.log(height) */
		/* 	console.log("=================") */
		/* } */
		
        Image {
            id: componentImage
            visible: itemIcon != ""
            source: itemIcon
            anchors.left: parent.left
			anchors.leftMargin: menu.menuPadding
			anchors.rightMargin: menu.menuPadding			
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: componentText
            visible: itemText != ""
            text: itemText
            font.pixelSize: 12
            anchors.left: componentImage.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: menu.picTextSpacing
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
                GradientStop { position: 0.5; color: "black" }
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
            onEntered: MenuItemJs.onEntered(parent, menu)
            onExited: MenuItemJs.onExited(parent, menu)
        }
    }
}
