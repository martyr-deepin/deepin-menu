import QtQuick 2.1

RectMenuWidget {
    id: menu
	
    fillColor: _menu_view.menuJsonObj.isDockMenu ? Qt.rgba(0, 0, 0, 0.6) : Qt.rgba(1, 1, 1, 1)
    fontColor: _menu_view.menuJsonObj.isDockMenu ? Qt.rgba(1, 1, 1, 1) : Qt.rgba(0, 0, 0, 1)			
    fontColorHover: _menu_view.menuJsonObj.isDockMenu ? "#00A4E2" : Qt.rgba(1, 1, 1, 1)		
    fontColorNotActive: _menu_view.menuJsonObj.isDockMenu ? "#646464" : "#b4b4b4"
    borderColor: _menu_view.menuJsonObj.isDockMenu ? Qt.rgba(1, 1, 1, 0.15) : Qt.rgba(0, 0, 0, 0.15)
    blurRadius: 16
    isDockMenu: _menu_view.menuJsonObj.isDockMenu || false
	isCheckableMenu: _menu_view.menuJsonObj.isCheckableMenu || false
	isSingleCheck: _menu_view.menuJsonObj.isSingleCheck || false
    menuJsonContent: _menu_view.menuJsonObj.menuJsonContent
}