import QtQuick 2.0

RectWithCorner {
    id: menu
    rectWidth: listview.width + blurWidth * 2
    rectHeight: listview.height + blurWidth * 2 + 2 * topBottomPadding
    cornerDirection: "down"

    property alias fontColor: listview.textColor
	property alias fontColorHover: listview.textColorHover
	property alias fontColorNotActive: listview.textColorNotActive
    property alias currentMenuIndex: listview.currentIndex
	property alias isCheckableMenu: listview.isCheckableMenu
	property alias isSingleCheck: listview.isSingleCheck
    property alias menuJsonContent: listview.menuJsonContent	
	property alias isDockMenu: listview.isDockMenu

    property int topBottomPadding: 3
	
	function updateCheckableItem(id, value) {
		listview.updateCheckableItem(id, value)
	}
	
	function updateItemActivity(id, value) {
		listview.updateItemActivity(id, value)
	}
	
	function selectItem(idx) {
		listview.currentIndex = idx
	}
	
    MenuItemListView {
        id: listview

        Component.onCompleted: {
            if (menu.cornerDirection == "down" || menu.cornerDirection == "up") {
				
				rectHeight += cornerHeight
                anchors.horizontalCenter = parent.horizontalCenter
				
                if (menu.cornerDirection == "down"){
					_menu_view.x -= cornerPos
					_menu_view.y -= menu.height
                    anchors.top = parent.top
                    anchors.topMargin = parent.blurWidth + topBottomPadding
                } else {
					_menu_view.x -= cornerPos
                    anchors.bottom = parent.bottom
                    anchors.bottomMargin = parent.blurWidth + topBottomPadding
                }

                var distance
                distance = 0 - _menu_view.x
                if (distance > 0) {
                    distance = Math.min(distance, menu.rectWidth - 2 * menu.blurWidth - menu.rectRadius - menu.cornerWidth / 2)
                    _menu_view.x += distance
                    menu.cornerPos -= distance
                }

                var distance = _menu_view.x + menu.width - _injection.getScreenWidth()
                if (distance > 0) {
                    distance = Math.min(distance, menu.rectWidth - 2 * menu.blurWidth - menu.rectRadius - menu.cornerWidth / 2)
                    _menu_view.x -= distance
                    menu.cornerPos += distance
                }
            }

            if (menu.cornerDirection == "left" || menu.cornerDirection == "right") {
				rectWidth += cornerHeight
                anchors.verticalCenter = parent.verticalCenter

                if (menu.cornerDirection == "right") {
					_menu_view.x -= _menu_view.width
					_menu_view.y -= cornerPos
                    anchors.left = parent.left
                    anchors.leftMargin = parent.blurWidth

                } else {
					_menu_view.y -= cornerPos
                    anchors.right = parent.right
                    anchors.rightMargin = parent.blurWidth
                }

                var distance
                distance = 0 - _menu_view.y
                if (distance > 0) {
                    distance = Math.min(distance, menu.rectHeight - 2 * menu.blurWidth - menu.rectRadius - menu.cornerWidth / 2)
                    _menu_view.y += distance
                    menu.cornerPos -= distance
                }

                var distance = menu.y + menu.height - _injection.getScreenHeight()
                if (distance > 0) {
                    distance = Math.min(distance, menu.rectHeight - 2 * menu.blurWidth - menu.rectRadius - menu.cornerWidth / 2)
                    _menu_view.y -= distance
                    menu.cornerPos += distance
                }
            }

        }
    }
}
