import QtQuick 2.2

ListView {
    id: listview
    width: 0
    height: 0
    focus: true
    currentIndex: -1
    interactive: true

    property Component selection: DesktopMenuSelection {}

    // style and predefined variables
    property int leftPadding: 5
    property int rightPadding: 5
    property int iconTextSpacing: 10
    property int textExtraMinSpacing: 10
    property int extraSubMenuIndicatorSpacing: 10

    property url checkmark: "images/check_light_normal.png"
    property url checkmarkHover: "images/check_light_hover.png"
    property url checkmarkPressed: "images/check_light_hover.png"
    property url checkmarkInactive: "images/check_light_inactive.png"

    property url arrow: "images/arrow-light.png"
    property url arrowHover: "images/arrow-light-hover.png"
    property url arrowPressed: "images/arrow-light-hover.png"
    property url arrowInactive: "images/arrow-light.png"

    property color itemTextColor: "black"
    property color itemTextHoverColor: "white"
    property color itemTextInactiveColor: "#b4b4b4"

    property color itemExtraColor: "#636363"
    property color itemExtraHoverColor: "#636363"
    property color itemExtraInactiveColor: "#636363"

    // logical variables
    property int maxTextWidth: 0
    property string naviDirection

    property bool isDockMenu: false

    signal enterPressed

    function setContent(content) {
        var itemContent = content.items

        for (var i = 0; i < itemContent.length; i++) {
            var pattern = /_(.)/
            var shortcutMatch = pattern.exec(itemContent[i].itemText)
            if (shortcutMatch != null) {
                itemContent[i].itemShortcut = shortcutMatch[1]
                itemContent[i].itemText = itemContent[i].itemText.replace(
                            pattern, "<u>" + shortcutMatch[1] + "</u>")
            }
            model.append({
                             itemId: itemContent[i].itemId,
                             itemIcon: itemContent[i].itemIcon,
                             itemIconHover: itemContent[i].itemIconHover,
                             itemIconInactive: itemContent[i].itemIconInactive,
                             itemText: itemContent[i].itemText,
                             itemShortcut: itemContent[i].itemShortcut || "",
                             itemExtra: itemContent[i].itemExtra || "",
                             itemSubMenu: itemContent[i].itemSubMenu,
                             isActive: itemContent[i].isActive,
                             checked: itemContent[i].checked
                         })
        }

        forceLayout()
    }

    function getNextItemsHasShortcut(startPos, key) {
        if (!key) return null

        for (var i = Math.max(startPos, 0); i < listview.count; i++) {
            // a trick here, using currentIndex as the cursor.
            if (key == listview.model.get(i).itemShortcut.toLowerCase()){
                return i
            }
        }

        for (var i = 0; i < listview.count; i++) {
            // we do the check another time to support wrapping.
            if (key == listview.model.get(i).itemShortcut.toLowerCase()) {
                return i
            }
        }
        return null
    }

    Keys.onEscapePressed: _utils_.menuDisappeared()
    Keys.onLeftPressed: global_menu.parentMenu.requestFocus()
    Keys.onRightPressed: global_menu.childMenu.requestFocus()
    Keys.onUpPressed: { naviDirection = "up"; event.accepted = false }
    Keys.onDownPressed: { naviDirection = "down"; event.accepted = false }
    Keys.onReturnPressed: { enterPressed() }
    Keys.onPressed: {
        var _next_index_has_shortcut = getNextItemsHasShortcut(currentIndex + 1, event.text)
        if (_next_index_has_shortcut != null) {
            listview.currentIndex = _next_index_has_shortcut
            if (_next_index_has_shortcut == getNextItemsHasShortcut(_next_index_has_shortcut + 1, event.text)) {
                enterPressed()
            }
        }
    }

    delegate: Item {
        id: item
        state: isActive ? "normal" : "inactive"
        width: listview.width
        height: isSeparator ? 6 : 24

        property bool isCheckable: itemId.split(":") != itemId
        property bool isSeparator: itemText == ""
        property bool hasSubMenu: itemSubMenu.items.length != 0

        states: [
            State {
                name: "normal"
                PropertyChanges {
                    target: selection_loader
                    active: false
                }
                PropertyChanges {
                    target: menu_item_text
                    color: listview.itemTextColor
                }
                PropertyChanges {
                    target: menu_item_extra
                    color: listview.itemExtraColor
                }
                PropertyChanges {
                    target: icon_image
                    source: item.isCheckable ? listview.checkmark : itemIcon
                }
                PropertyChanges {
                    target: sub_menu_indicator
                    source: item.hasSubMenu ? listview.arrow : ""
                }
            },
            State {
                name: "hover"
                PropertyChanges {
                    target: selection_loader
                    active: true
                }
                PropertyChanges {
                    target: menu_item_text
                    color: listview.itemTextHoverColor
                }
                PropertyChanges {
                    target: menu_item_extra
                    color: listview.itemExtraHoverColor
                }
                PropertyChanges {
                    target: icon_image
                    source: item.isCheckable ? listview.checkmarkHover : itemIconHover
                }
                PropertyChanges {
                    target: sub_menu_indicator
                    source: item.hasSubMenu ? listview.arrowHover : ""
                }
            },
            State {
                name: "inactive"
                PropertyChanges {
                    target: selection_loader
                    active: false
                }
                PropertyChanges {
                    target: menu_item_text
                    color: listview.itemTextInactiveColor
                }
                PropertyChanges {
                    target: menu_item_extra
                    color: listview.itemExtraInactiveColor
                }
                PropertyChanges {
                    target: icon_image
                    source: item.isCheckable ? listview.checkmarkInactive : itemIconInactive
                }
                PropertyChanges {
                    target: sub_menu_indicator
                    source: item.hasSubMenu ? listview.arrowInactive : ""
                }
            }
        ]

        Connections {
            target: listview
            onCurrentIndexChanged: {
                if (listview.currentIndex == index) {
                    if (isActive && !item.isSeparator) {
                        item.state = "hover"
                    } else {
                        item.state = "inactive"
                        listview.naviDirection == "down" ? listview.currentIndex += 1 : ""
                        listview.naviDirection == "up" ? listview.currentIndex -= 1 : ""
                    }
                    item.showSubMenu()
                } else if (listview.currentIndex != -1) {
                    item.state = isActive ? "normal" : "inactive"
                }
            }
            onEnterPressed: {
                if (listview.currentIndex == index) {
                    item.doClicked()
                }
            }
        }

        function doClicked() {
            if (isActive && !item.isSeparator && !item.hasSubMenu) {
                _utils_.itemClicked(itemId, !checked)

                if (item.isCheckable) {
                    global_screen.setChecked(itemId, !checked)
                } else {
                    _utils_.menuDisappeared()
                }
            }
        }

        function showSubMenu() {
            global_menu.destroySubMenu()

            var itemPos = item.mapToItem(global_screen, 0, 0)
            isActive && item.hasSubMenu && global_menu.showSubMenu(itemPos.x + item.width,
                                                                   itemPos.y, itemSubMenu)
        }

        Loader {
            id: selection_loader
            visible: !item.isSeparator
            sourceComponent: listview.selection
            anchors.fill: parent
        }

        Image {
            id: icon_image
            width: 14
            height: 14
            visible: item.isCheckable ? checked : true

            anchors.left: parent.left
            anchors.leftMargin: listview.leftPadding
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: menu_item_text
            text: itemText
            width: Math.max(implicitWidth, listview.maxTextWidth)

            anchors.left: icon_image.right
            anchors.leftMargin: listview.iconTextSpacing
            anchors.verticalCenter: parent.verticalCenter

            onWidthChanged: listview.width = Math.max(
                                (listview.textExtraMinSpacing + icon_image.width
                                 + icon_image.anchors.leftMargin + menu_item_text.width
                                 + menu_item_text.anchors.leftMargin + menu_item_extra.implicitWidth
                                 + menu_item_extra.anchors.rightMargin + sub_menu_indicator.width
                                 + sub_menu_indicator.anchors.rightMargin),
                                listview.width)
            Component.onCompleted: listview.maxTextWidth = Math.max(
                                       width, listview.maxTextWidth)
        }

        Text {
            id: menu_item_extra
            text: itemExtra

            anchors.right: sub_menu_indicator.left
            anchors.rightMargin: listview.extraSubMenuIndicatorSpacing
            anchors.verticalCenter: parent.verticalCenter
        }

        Image {
            id: sub_menu_indicator
            width: 14
            height: 14
            visible: item.hasSubMenu

            anchors.right: parent.right
            anchors.rightMargin: listview.rightPadding
            anchors.verticalCenter: parent.verticalCenter
        }

        Item {
            id: separator
            width: parent.width
            height: 2
            visible: item.isSeparator
            anchors.centerIn: parent

            Rectangle {
                x: 0
                y: 1
                width: 1
                height: parent.width

                transformOrigin: Item.TopLeft
                rotation: -90
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.rgba(0, 0, 0, 0.2) }
                    GradientStop { position: 0.5; color: Qt.rgba(0, 0, 0, 0.25) }
                    GradientStop { position: 1.0; color: Qt.rgba(0, 0, 0, 0.2) }
                }
            }

            Rectangle {
                x: 0
                y: 2
                width: 1
                height: parent.width

                transformOrigin: Item.Center
                rotation: -90
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.rgba(1, 1, 1, 0.1)}
                    GradientStop { position: 0.5; color: Qt.rgba(1, 1, 1, 0.15) }
                    GradientStop { position: 1.0; color: Qt.rgba(1, 1, 1, 0.1) }
                }

                anchors.centerIn: parent
            }
        }

        Component.onCompleted: {
            listview.width = Math.max(
                        (listview.textExtraMinSpacing + icon_image.width
                         + icon_image.anchors.leftMargin + menu_item_text.width
                         + menu_item_text.anchors.leftMargin + menu_item_extra.implicitWidth
                         + menu_item_extra.anchors.rightMargin + sub_menu_indicator.width
                         + sub_menu_indicator.anchors.rightMargin),
                        listview.width)
            listview.height += height
        }

        MouseArea {
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                global_menu.stopDestroy()

                listview.currentIndex = index
            }
            onExited: {
                listview.currentIndex = -1
            }
            onClicked: {
                item.doClicked()
            }
        }
    }

    model: ListModel {
    }
}
