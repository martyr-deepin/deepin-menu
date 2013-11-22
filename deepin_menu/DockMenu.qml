import QtQuick 2.1

DockMenuWidget {
    id: menu

    fillColor: Qt.rgba(0, 0, 0, 0.6)
    fontColor: Qt.rgba(1, 1, 1, 1)
    fontColorHover: "#00A4E2"
    fontColorNotActive: "#646464"
    borderColor: Qt.rgba(1, 1, 1, 0.15)
    blurRadius: 16
    isDockMenu: true
    menuJsonContent: _menu_view.menuJsonContent
}