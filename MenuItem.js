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

function onEntered(menuItem, menu, fullscreen_bg) {
    menuItem.color = Qt.rgba(0, 100, 0, 0.7)

	// Destroy old subMenu
    if (menu.subMenuObj != null) {
        menu.subMenuObj.destroy(10)
		menu.subMenuObj = null
    }

	// Create new subMenu
    if (menuItem.componentSubMenu != "[]") {
        if (menuItem.componentSubMenu != null) {
            var component = Qt.createComponent("RectMenu.qml");
            var component_menuItems = menuItem.componentSubMenu
			
			var component_size = menuItem.ListView.view.getSize(component_menuItems)
            var component_width = component_size.width
            var component_height = component_size.height

            var component_x = menu.x + menu.width
            var component_y = menu.y + menuItem.y

            if (component_x > fullscreen_bg.width) {
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

function onExited(menuItem, menu) {
    menuItem.color = Qt.rgba(1, 1, 1, 0)
}
