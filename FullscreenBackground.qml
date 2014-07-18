import QtQuick 2.0
import QtQuick.Window 2.0

Item {
    id: global_screen
    width: Screen.width
    height: Screen.height

    function showMenu(x, y, content) {
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

        content = JSON.stringify([checkbox1, checkbox2, radio1, radio2, edit])

        desktop_menu.x = x
        desktop_menu.y = y

        desktop_menu.setContent(content)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: Qt.quit()
    }

    DesktopMenu {
        id: desktop_menu
    }
}
