import QtQuick 2.1

RectMenuWidget {
    id: menu
	
    fillColor: _menu_view.menuJsonObj.fillColor
    fontColor: _menu_view.menuJsonObj.fontColor
    fontColorHover: _menu_view.menuJsonObj.fontColorHover
    fontColorNotActive: _menu_view.menuJsonObj.fontColorNotActive
    borderColor: _menu_view.menuJsonObj.borderColor
    blurRadius: _menu_view.menuJsonObj.blurRadius
    isDockMenu: false
    menuJsonContent: _menu_view.menuJsonObj.menuJsonContent
}