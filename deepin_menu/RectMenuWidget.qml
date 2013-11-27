import QtQuick 2.0

Rect {
    id: menu
    rectWidth: listview.width + blurWidth * 2
    rectHeight: listview.height + blurWidth * 2 + 2 * topBottomPadding

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
		listview.currentItem.componentSubMenu = listview.updateCheckableItem(listview.currentItem.componentSubMenu, id, value)
	}
	
	function selectItem(idx) {
		listview.currentIndex = idx
	}
	
    MenuItemListView {
        id: listview

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: parent.blurWidth + topBottomPadding
        }
    }
}
