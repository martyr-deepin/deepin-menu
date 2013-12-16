import QtQuick 2.0

ListModel {
    id: menuItemModel

    property string menuJsonContent: ""

    function updateMenuJsonContent(menuJsonContent) {
		menuItemModel.clear()
		
        var menu = JSON.parse(menuJsonContent)
        var items = menu.items
        var _width = 0
        var _height = 0
        for (var i in items) {
            var pattern = /_(.)/
            var shortcutMatch = pattern.exec(items[i].itemText)
            if (shortcutMatch != null) {
                items[i].itemShortcut = shortcutMatch[1]
                items[i].itemText = items[i].itemText.replace(pattern, "<u>" + shortcutMatch[1] + "</u>")
            }
            menuItemModel.append({"itemId": items[i].itemId,
                                  "itemIcon":items[i].itemIcon,
                                  "itemIconHover":items[i].itemIconHover,
                                  "itemIconInactive":items[i].itemIconInactive,
                                  "itemText":items[i].itemText,
                                  "itemShortcut": items[i].itemShortcut || "",
                                  "itemSubMenu":JSON.stringify(items[i].itemSubMenu),
                                  "isActive": items[i].isActive,
                                  "isCheckable": items[i].isCheckable,
                                  "checked": items[i].checked,
                                  "showCheckmark": items[i].showCheckmark})
        }
    }

    Component.onCompleted: {
		updateMenuJsonContent(menuJsonContent)
    }
}
