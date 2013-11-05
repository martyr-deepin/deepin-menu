import QtQuick 2.0

Rectangle{
    id: menu
    width: 200
    height: 300
    color: Qt.rgba(0, 0, 0, 0)

    ListModel {
        id: menuItemModel

        Component.onCompleted: {
            menuItemModel.append({itemIcon:"/usr/share/icons/Deepin/apps/16/preferences-display.png", itemText:"Display"})
            menuItemModel.append({itemIcon:"/usr/share/icons/Deepin/apps/16/preferences-driver.png", itemText:"Driver"})
			/* console.log(_menu_view.getItemsNum()) */
			/* console.log(_menu_view.getItem(_menu_view, 0)) */
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(50, 50, 50, 0.7)
        radius: 3
    }

    ListView {
        id: menuItems
        width: parent.width
        height: parent.height
        delegate: MenuItemDelegate {}
        model: menuItemModel
        interactive: false
        currentIndex: 0
    }
}
