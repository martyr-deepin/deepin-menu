import QtQuick 2.0

AbstractMenu {
    id: global_menu
    menuContent: content
    glowRadius: desktop_menu_background.glowRadius

    DockMenuBackground {
        id: desktop_menu_background
        MenuContent {
            id: content

            selection: Item{}

            checkmark: "images/check_dark_normal.png"
            checkmarkHover: "images/check_dark_hover.png"
            checkmarkPressed: "images/check_dark_hover.png"
            checkmarkInactive: "images/check_dark_inactive.png"

            itemTextColor: "white"
            itemTextHoverColor: "#00A4E2"
            itemTextInactiveColor: "#646464"
        }
    }
}
