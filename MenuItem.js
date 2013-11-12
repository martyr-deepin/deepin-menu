function onPressed(menuItem, menu) {
	if (menuItem.componentSubMenu == "[]") {
		// menuItem.color = Qt.rgba(0, 100, 0, 0.9)
		_menu_view.invokeItem(parseInt(menuItem.componentId))
		_application.quit()
	}
}

function onEntered(index, menu) {
	menu.currentMenuIndex = index
}
