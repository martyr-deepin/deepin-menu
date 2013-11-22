import QtQuick 2.0

ListModel {
    id: menuItemModel
	
	property string menuJsonContent: ""
	
    Component.onCompleted: {
        var menu = JSON.parse(menuJsonContent)
		var items = menu.items
        var _width = 0
        var _height = 0
        for (var i in items) {
            menuItemModel.append({"itemId": items[i].itemId,
                                  "itemIcon":items[i].itemIcon,
                                  "itemIconHover":items[i].itemIconHover,
                                  "itemText":items[i].itemText,
                                  "itemSubMenu":JSON.stringify(items[i].itemSubMenu),
								  "isActive": items[i].isActive,
								  "isCheckable": items[i].isCheckable,
								  "checked": items[i].checked})
		}
    }
}
