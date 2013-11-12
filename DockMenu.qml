import QtQuick 2.0

RectWithCorner {
    id: menu
    rectWidth: listview.width + blurWidth * 2
    rectHeight: listview.height + blurWidth * 2 + cornerHeight + 2 * topBottomPadding
	
	property alias fontColor: listview.textColor	
	property alias currentMenuIndex: listview.currentIndex	
	property int topBottomPadding: 3
	property string menuItems: ""

    property var subMenuObj: null
	property var fullscreenBg: null

    Component.onDestruction: {
        if (menu.subMenuObj != null) {
            menu.subMenuObj.destroy()
        }
    }

    MenuItemListView {
        id: listview

		fullscreenBg: parent.fullscreenBg
        menuItems: parent.menuItems

        anchors {
			horizontalCenter: parent.horizontalCenter
			top: parent.top
			topMargin: parent.blurWidth + topBottomPadding
		}
    }
}
