function onPressed(menuItem, menu) {
	if (menuItem.componentSubMenu == "[]") {
		menuItem.color = Qt.rgba(0, 100, 0, 0.9)
		_menu_view.invokeItem(parseInt(menuItem.componentId))
		_application.quit()
	}
}

function onReleased(menuItem, menu) {
    menuItem.color = Qt.rgba(1, 1, 1, 0)
}

function onEntered(menuItem, menu) {
    menuItem.color = Qt.rgba(0, 100, 0, 0.7)

	// Destroy old subMenu
    if (menu.subMenuObj != null) {
        menu.subMenuObj.destroy(10)
		menu.subMenuObj = null
    }

	// Create new subMenu
    if (menuItem.componentSubMenu != "[]") {
        if (menuItem.componentSubMenu != null) {
            var component = Qt.createComponent("ActualMenu.qml");
            var component_menuItems = menuItem.componentSubMenu
            var component_width = 200
            var component_height = 400
            var component_x = menu.x + menu.width
            var component_y = menu.y + menuItem.y

            if (component_x > fullscreen_bg.width) {
                component_x = menu.x - component_width
            }

            if (component_y + component_height > fullscreen_bg.height) {
                component_y = fullscreen_bg.height - component_height
            }

            var obj = component.createObject(fullscreen_bg, {"x": component_x, "y": component_y,
                                                             "width": component_width, "height": component_height,
                                                             "menuItems": component_menuItems});

            menu.subMenuObj = obj
        }
    }
}

function onExited(menuItem, menu) {
    menuItem.color = Qt.rgba(1, 1, 1, 0)
}


function getMenuWidthByLabelLen(len) {
    return len * 12
}

function getMenuHeightByItemsNum(num) {
    return num * 12
}
