import QtQuick 2.0
import QtQuick.Window 2.0

Item {
    id: global_screen
    width: Screen.width
    height: Screen.height

    property int desktopAvailableWidth: Screen.desktopAvailableWidth
    property int desktopAvailableHeight: Screen.desktopAvailableHeight

    function showMenu(menuJsonContent) {
        var checkbox1 = {
            "itemId": "checkbox:checkbox:checkbox_1",
            "itemText": "Checkbox One",
            "itemIcon": "",
            "itemIconHover": "",
            "itemIconPressed": "",
            "itemExtra": "Ctrl-C",
            "itemSubMenu": "[]"
        }
        var checkbox2 = {
            "itemId": "checkbox:checkbox:checkbox_1",
            "itemText": "Checkbox One",
            "itemIcon": "",
            "itemIconHover": "",
            "itemIconPressed": "",
            "itemExtra": "Ctrl-C",
            "itemSubMenu": "[]"
        }
        var radio1 = {
            "itemId": "radio:radio:radio_1",
            "itemText": "Radio One",
            "itemIcon": "",
            "itemIconHover": "",
            "itemIconPressed": "",
            "itemExtra": "Ctrl-C",
            "itemSubMenu": "[]"
        }
        var radio2 = {
            "itemId": "radio:radio:radio_2",
            "itemText": "Radio Two",
            "itemIcon": "",
            "itemIconHover": "",
            "itemIconPressed": "",
            "itemExtra": "Ctrl-C",
            "itemSubMenu": "[]"
        }
        var copy = {
            "itemId": "copy",
            "itemText": "Copy",
            "itemIcon": "",
            "itemIconHover": "",
            "itemIconPressed": "",
            "itemExtra": "Ctrl-C",
            "itemSubMenu": "[]"
        }
        var paste = {
            "itemId": "paste",
            "itemText": "Paste",
            "itemIcon": "",
            "itemIconHover": "",
            "itemIconPressed": "",
            "itemExtra": "Ctrl-V",
            "itemSubMenu": "[]"
        }
        var edit = {
            "itemId": "edit",
            "itemText": "Edit",
            "itemIcon": "",
            "itemIconHover": "",
            "itemIconPressed": "",
            "itemExtra": "",
            "itemSubMenu": JSON.stringify([copy, paste])
        }

        var content = JSON.stringify([checkbox1, checkbox2, radio1, radio2, edit])

        desktop_menu.x = 1200
        desktop_menu.y = 300

        desktop_menu.setContent(content)

//        var content = JSON.parse(menuJsonContent);

//        desktop_menu.x = content.x
//        desktop_menu.y = content.y

//        desktop_menu.setContent(content.menuJsonContent)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: Qt.quit()
    }

    DesktopMenu {
        id: desktop_menu
    }
}
