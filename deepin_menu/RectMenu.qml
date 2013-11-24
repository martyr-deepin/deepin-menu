import QtQuick 2.1

RectMenuWidget {
    id: menu

    fillColor: Qt.rgba(1, 1, 1, 0.9)
    fontColor: Qt.rgba(0, 0, 0, 1)
    fontColorHover: Qt.rgba(0, 0, 0, 1)
    fontColorNotActive: "#b4b4b4"
    borderColor: Qt.rgba(0, 0, 0, 0.15)
    blurRadius: 16
    isDockMenu: false
    menuJsonContent: _menu_view.menuJsonObj.menuJsonContent
}