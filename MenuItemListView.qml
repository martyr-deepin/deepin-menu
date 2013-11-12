import QtQuick 2.0

ListView {
    id: listview
    focus: true                 /* to enable key navigation */
    currentIndex: -1
    model: MenuItemListModel { menuItems: listview.menuItems}
    delegate: MenuItemDelegate {}
    highlight: Rectangle { color: "lightsteelblue" }
    /* keyNavigationWraps: true */
    /* interactive: false */

    property var fullscreenBg: null
    property int textSize: 12
    property int textLeftMargin: 22
    property int horizontalPadding: 3
    property int verticalPadding: 3
    property int pictureSize: 16
    property int minWidth: 100

    property color textColor: Qt.rgba(1, 1, 1, 1)

    property string menuItems: ""

    property int lastCurrentIndex: 0
    onCurrentItemChanged: {
        /* console.log(currentItem.isSep) */
        if (currentItem.isSep) {
            if (currentIndex > lastCurrentIndex) {
                currentIndex += 1
            }
            if (currentIndex < lastCurrentIndex) {
                currentIndex -= 1
            }
            lastCurrentIndex = currentIndex
        }

        // Destroy old subMenu
        if (menu.subMenuObj != null) {
            menu.subMenuObj.destroy(10)
            menu.subMenuObj = null
        }

        // Create new subMenu
        if (currentItem.componentSubMenu != "[]") {
            if (currentItem.componentSubMenu != null) {
                var component = Qt.createComponent("RectMenu.qml");
                var component_menuItems = currentItem.componentSubMenu

                var component_size = currentItem.ListView.view.getSize(component_menuItems)
                var component_width = component_size.width
                var component_height = component_size.height

                var component_x = menu.x + menu.width
                var component_y = menu.y + currentItem.y

                if (component_x + component_width> fullscreen_bg.width) {
                    component_x = menu.x - component_width
                }

                if (component_y + component_height > fullscreen_bg.height) {
                    component_y = fullscreen_bg.height - component_height
                }

                var obj = component.createObject(fullscreen_bg, {"x": component_x, "y": component_y,
                                                                 // "width": component_width, "height": component_height,
                                                                 "menuItems": component_menuItems, "fullscreenBg": fullscreenBg});
                menu.subMenuObj = obj
            }
        }
    }

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
                _height += verticalPadding * 2 + 2
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
