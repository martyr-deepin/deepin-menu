function onPressed(index, menuItem, menu) {
	if (JSON.parse(menuItem.componentSubMenu).items.length == 0) {
		_menu_view.invokeItem(menuItem.componentId)
		
		if (menuItem.componentCheckable) {
			if (menu.isSingleCheck) {
				menuItem.ListView.view.itemChecked(index)
			}
			menuItem.componentChecked = !menuItem.componentChecked
			menuItem.itemIconPic = menuItem.iconHover
			return
		}
		_application.quit()
	}
}

function onEntered(index, menuItem, menu) {
	menu.currentMenuIndex = index	
}
