import QtQuick 2.0

ListModel {
    id: menuItemModel

    property string menuJsonContent: ""
    
    function _update_item_checked(json_obj, id, value) {
        for (var item in json_obj.items) {
            if (json_obj.items[item].itemId == id) {
                json_obj.items[item].checked = value
            }
            if (json_obj.items[item].itemSubMenu.items.length != 0) {
                _update_item_checked(json_obj.items[item].itemSubMenu, id, value)
            }
        }
    }
    
    function updateItemChecked(id, value) {
        for (var i = 0; i < menuItemModel.count; i++) {
            if (menuItemModel.get(i).itemId == id) {
                menuItemModel.setProperty(i, "checked", value)
                return
            } 
            var sub_obj = JSON.parse(menuItemModel.get(i).itemSubMenu)
            if (sub_obj.items.length != 0) {
                _update_item_checked(sub_obj, id, value)
                menuItemModel.setProperty(i, "itemSubMenu", JSON.stringify(sub_obj))
            }
        }
    }
    
    function _update_item_text(json_obj, id, value) {
        for (var item in json_obj.items) {
            if (json_obj.items[item].itemId == id) {
                json_obj.items[item].itemText = value
            }
            if (json_obj.items[item].itemSubMenu.items.length != 0) {
                _update_item_text(json_obj.items[item].itemSubMenu, id, value)
            }
        }
    }
    
    function updateItemText(id, value) {
        for (var i = 0; i < menuItemModel.count; i++) {
            if (menuItemModel.get(i).itemId == id) {
                menuItemModel.setProperty(i, "itemText", value)                
                return
            }
            var sub_obj = JSON.parse(menuItemModel.get(i).itemSubMenu)
            if (sub_obj.items.length != 0) {
                _update_item_text(sub_obj, id, value)
                menuItemModel.setProperty(i, "itemSubMenu", JSON.stringify(sub_obj))                
            }
        }
    }
    
    function _update_item_activity(json_obj, id, value) {
        for (var item in json_obj.items) {
            if (json_obj.items[item].itemId == id) {
                json_obj.items[item].isActive = value
            }
            if (json_obj.items[item].itemSubMenu.items.length != 0) {
                _update_item_activity(json_obj.items[item].itemSubMenu, id, value)
            }
        }
    }
    
    function updateItemActivity(id, value) {
        for (var i = 0; i < menuItemModel.count; i++) {
            if (menuItemModel.get(i).itemId == id) {
                menuItemModel.setProperty(i, "isActive", value)                
                return
            }
            var sub_obj = JSON.parse(menuItemModel.get(i).itemSubMenu)
            if (sub_obj.items.length != 0) {
                _update_item_activity(sub_obj, id, value)
                menuItemModel.setProperty(i, "itemSubMenu", JSON.stringify(sub_obj))                
            }
        }
    }
    
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
                                  "itemExtra": items[i].itemExtra || "",
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
