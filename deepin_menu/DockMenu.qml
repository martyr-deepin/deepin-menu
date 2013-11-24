import QtQuick 2.1

DockMenuWidget {
    id: menu

    fillColor: _menu_view.menuJsonObj.fillColor
    fontColor: _menu_view.menuJsonObj.fontColor
    fontColorHover: _menu_view.menuJsonObj.fontColorHover
    fontColorNotActive: _menu_view.menuJsonObj.fontColorNotActive
    borderColor: _menu_view.menuJsonObj.borderColor
    blurRadius: _menu_view.menuJsonObj.blurRadius
    isDockMenu: true
	cornerDirection: _menu_view.menuJsonObj.cornerDirection
    menuJsonContent: _menu_view.menuJsonObj.menuJsonContent
}