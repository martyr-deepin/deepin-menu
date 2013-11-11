import QtQuick 2.0
import QtGraphicalEffects 1.0

import "MenuItem.js" as MenuItemJs

Rectangle{
    id: menu
    radius: 3
    color: Qt.rgba(50, 50, 50, 0.7)

    property int minWidth: 100
    property int picWidth: 16
    property int picTextSpacing: 2
    property int menuPadding: 5
    property int menuItemPadding: 2

    property string menuItems: null
    property var subMenuObj: null

    anchors.centerIn: parent

    Component.onDestruction: {
        if (menu.subMenuObj != null) {
            menu.subMenuObj.destroy()
        }
    }

    ListModel {
        id: menuItemModel

        Component.onCompleted: {
            var items = JSON.parse(menu.menuItems)
            var _width = 0
            var _height = 0
            for (var i in items) {
                menuItemModel.append({"itemId": items[i].itemId,
                                      "itemIcon":items[i].itemIcon,
                                      "itemText":items[i].itemText,
                                      "itemSubMenu":JSON.stringify(items[i].itemSubMenu)})
                if (_injection.getStringWidth(items[i].itemText, 12)
                    + menu.picWidth + menu.picTextSpacing + menu.menuPadding * 2 > _width) {
                    _width = _injection.getStringWidth(items[i].itemText, 12)
                    + menu.picWidth + menu.picTextSpacing + menu.menuPadding * 2
                }

                if (items[i].itemText == undefined) {
                    _height += menu.menuItemPadding * 2 + 1
                } else {

                    _height += Math.max(_injection.getStringHeight(items[i].itemText, 12) + menu.menuItemPadding * 2 + 5,
                                        menu.picWidth + menu.menuItemPadding * 2 + 5)
                }

            /* if (_injection.getStringHeight(items)) */
            }
            menu.width = (Math.max(_width, menu.minWidth))
            /* console.log(_height) */
            menu.height = _height + 2 * menu.menuPadding - 5
        /* menu.height = 100 */
        }
    }

    ListView {
        id: menuItems
        spacing: 5
        delegate: MenuItemDelegate {}
        model: menuItemModel
        interactive: false
        /* currentIndex: 0 */
        anchors {
            fill: parent
            topMargin: menu.menuPadding
            bottomMargin: menu.menuPadding
        }
    }
}
