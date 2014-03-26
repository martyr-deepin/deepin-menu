function onClicked(index, menuItem, menu) {
    if (JSON.parse(menuItem.componentSubMenu).items.length == 0 && menuItem.componentActive == true) {
        _menu_view.invokeItem(menuItem.componentId, menuItem.componentCheckable && !menuItem.componentChecked);

        if (menuItem.componentCheckable) {

            menuItem.componentChecked = !menuItem.componentChecked;
            if (menuItem.componentChecked) {
                menuItem.ListView.view.itemChecked(index, menuItem)
            } else {
                menuItem.ListView.view.itemUnchecked(index, menuItem)
            }
			_menu_view.notifyUpdateItemChecked(menuItem.componentId, menuItem.componentChecked);

            menuItem.itemIconPic = menuItem.iconHover;
            return
        }
        _menu_view.destroyWholeMenu()
    }
}

function onEntered(index, menuItem, menu) {
    if (menuItem.componentActive && !menuItem.isSep) {
        menu.currentMenuIndex = index;
    }
}
