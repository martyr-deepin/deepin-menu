function onPressed(menuItem) {
	menuItem.color = Qt.rgba(0, 100, 0, 0.9) 
	_menu_view.showSubMenu(0)
}

function onReleased(menuItem) {
	menuItem.color = Qt.rgba(1, 1, 1, 0) 
}

function onEntered(menuItem) {
	menuItem.color = Qt.rgba(0, 100, 0, 0.7) 
}

function onExited(menuItem) {
	menuItem.color = Qt.rgba(1, 1, 1, 0)
}
