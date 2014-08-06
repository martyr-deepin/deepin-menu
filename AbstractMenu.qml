import QtQuick 2.2
import QtQuick.Window 2.0

Item {
    id: abstract_menu
    width: childrenRect.width
    height: childrenRect.height

    property QtObject parentMenu: null
    property QtObject childMenu: null
    property int glowRadius: 0

    default property alias content: content_item.children
    property QtObject menuContent

    property int originX
    property int originY
    onWidthChanged: {
        if (originX + width - glowRadius > global_screen.desktopAvailableWidth) {
            x = parentMenu ? parentMenu.x - width + 2 * glowRadius
                           : global_screen.desktopAvailableWidth - width + glowRadius
        }
    }
    onHeightChanged: {
        if (originY + height - glowRadius > global_screen.desktopAvailableHeight) {
            y = global_screen.desktopAvailableHeight - height + glowRadius
        }
    }

    function setContent(contnt) {
        menuContent.setContent(contnt)
    }

    function requestFocus() { menuContent.focus = true }

    function startDestroy() { destroy_timer.restart() }
    function stopDestroy() { destroy_timer.stop() }

    function destroySubMenu() {
        if (childMenu) {
            childMenu.destroySubMenu()
            childMenu.destroy()
        }
    }

    function destroySubMenuLater() {
        if (childMenu) {
            childMenu.destroySubMenu()
            childMenu.startDestroy()
        }
    }

    function _updateMenuChecked(groupId, groupType, itemId, checked, menu) {
        for (var i = 0; i < menu.items.count; i++) {
            var itemInfo = menu.items[i].itemId.split(":")
            if (itemInfo.length == 3) {
                var itemGroupId = itemInfo[0]
                var itemGroupType = itemInfo[1]
                var itemItemId = itemInfo[2]

                if (itemItemId != itemId) {
                    if (itemGroupId == groupId
                            && itemGroupType == "radio")
                    {
                        checked && (menu.items[i].checked = false)
                    }
                } else {
                    menu.items[i].checked = checked
                }
            }

            menu.items[i].itemSubMenu = _updateMenuChecked(groupId,
                                                           groupType,
                                                           itemId,
                                                           checked,
                                                           menu.items[i].itemSubMenu)
        }

        return menu
    }

    function setChecked(id, checked) {
        var destItemInfo = id.split(":")
        var destItemGroupId = destItemInfo[0]
        var destItemGroupType = destItemInfo[1]
        var destItemItemId = destItemInfo[2]

        // update UI
        for (var i = 0; i < menuContent.count; i++) {
            var itemInfo = menuContent.model.get(i).itemId.split(":")
            if (itemInfo.length == 3) {
                var itemGroupId = itemInfo[0]
                var itemGroupType = itemInfo[1]
                var itemItemId = itemInfo[2]

                if (itemItemId != destItemItemId) {
                    if (itemGroupId == destItemGroupId
                            && itemGroupType == "radio")
                    {
                        checked && (menuContent.model.get(i).checked = false)
                    }
                } else {
                    menuContent.model.get(i).checked = checked
                }
            }
        }

        // update content
        for (var i = 0; i < menuContent.count; i++) {
            var itemSubMenu = menuContent.model.get(i).itemSubMenu
            menuContent.model.setProperty(i, "itemSubMenu",
                                          _updateMenuChecked(destItemGroupId,
                                                             destItemGroupType,
                                                             destItemItemId,
                                                             checked,
                                                             itemSubMenu))
        }

        // update subMenu
        childMenu && childMenu.setChecked(id, checked)
    }

    function _updateMenuText(id, text, menu) {
        for (var i = 0; i < menu.items.count; i++) {
            if (menu.items[i].itemId == id) {
                menu.items[i].itemText = text
                break
            }

            menu.items[i].itemSubMenu = _updateMenuChecked(id,
                                                           text,
                                                           menu.items[i].itemSubMenu)
        }

        return menu
    }

    function setText(id, text) {
        // update UI
        for (var i = 0; i < menuContent.count; i++) {
            if (menuContent.model.get(i).itemId = id) {
                menuContent.model.setProperty(i, "itemText", text)
            }
        }

        // update content
        for (var i = 0; i < menuContent.count; i++) {
            var itemSubMenu = menuContent.model.get(i).itemSubMenu
            menuContent.model.setProperty(i, "itemSubMenu",
                                          _updateMenuText(id,
                                                          checked,
                                                          itemSubMenu))
        }

        // update subMenu
        childMenu && childMenu.setText(id, text)
    }

    function _updateMenuActive(id, active, menu) {
        for (var i = 0; i < menu.items.count; i++) {
            if (menu.items[i].itemId == id) {
                menu.items[i].isActive = active
                break
            }

            menu.items[i].itemSubMenu = _updateMenuChecked(id,
                                                           active,
                                                           menu.items[i].itemSubMenu)
        }

        return menu
    }

    function setActive(id, active) {
        // update UI
        for (var i = 0; i < menuContent.count; i++) {
            if (menuContent.model.get(i).itemId == id) {
                menuContent.model.setProperty(i, "isActive", active)
            }
        }

        // update content
        for (var i = 0; i < menuContent.count; i++) {
            var itemSubMenu = menuContent.model.get(i).itemSubMenu
            menuContent.model.setProperty(i, "itemSubMenu",
                                          _updateMenuActive(id,
                                                            active,
                                                            itemSubMenu))
        }

        // update subMenu
        childMenu && childMenu.setActive(id, active)
    }

    Timer {
        id: destroy_timer
        interval: 100
        onTriggered: abstract_menu.destroySubMenu()
    }

    Item {
        id: content_item
        width: childrenRect.width
        height: childrenRect.height
    }
}
