import QtQuick 2.0

ListView {
    id: listview
    model: MenuItemListModel { menuItems: listview.menuItems}
    delegate: MenuItemDelegate {}
    interactive: false

    property var fullscreenBg: null
    property int textSize: 12
    property int textLeftMargin: 22
    property int horizontalPadding: 3
    property int verticalPadding: 3
    property int pictureSize: 16
    property int minWidth: 100

    property color textColor: Qt.rgba(1, 1, 1, 1)

    property string menuItems: ""

    function getSize() {
        var items = JSON.parse(menuItems)
        var _width = 0
        var _height = 0
        for (var i in items) {
            if (_injection.getStringWidth(items[i].itemText, textSize)
                + textLeftMargin + horizontalPadding > _width) {
                _width = _injection.getStringWidth(items[i].itemText, textSize)
                + textLeftMargin + horizontalPadding
            }

            if (items[i].itemText == undefined) {
                _height += verticalPadding * 2 + 1
            } else {
                _height += Math.max(_injection.getStringHeight(items[i].itemText, textSize) + verticalPadding * 2,
                                    pictureSize + verticalPadding * 2)
            }
        }
		
		return {"width": _width, "height": _height}
    }

    Component.onCompleted: {
		var size = getSize()
		
        listview.width = (Math.max(size.width, minWidth))
        listview.height = size.height
    }
}
