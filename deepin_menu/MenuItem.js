function onPressed(menuItem, menu) {
	if (menuItem.componentSubMenu == "[]") {
		_menu_view.invokeItem(menuItem.componentId)
		
		if (menuItem.componentCheckable) {
			menuItem.componentChecked = !menuItem.componentChecked
			menuItem.itemIconPic = menuItem.iconHover
			return
		}
		_application.quit()
	}
}

function onEntered(index, menu) {
	menu.currentMenuIndex = index
}
