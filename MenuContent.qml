import QtQuick 2.2

ListView {
    id: listview
    width: 0
    height: 0
    focus: true
    interactive: true

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

    property color itemColor: "transparent"
    property color itemHoverColor: "blue"
    property color itemPressedColor: "blue"
    property color itemInactiveColor: "transparent"

    property color itemTextColor: "black"
    property color itemTextHoverColor: "red"
    property color itemTextPressedColor: "blue"
    property color itemTextInactiveColor: "grey"

    // logical variables
    property int maxTextWidth: 0

    function setContent(content) {
        var itemContent = JSON.parse(content)
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
                             itemIconPressed: itemContent[i].itemIconHover,
                             itemIconInactive: itemContent[i].itemIconInactive,
                             itemText: itemContent[i].itemText,
                             itemShortcut: itemContent[i].itemShortcut || "",
                             itemExtra: itemContent[i].itemExtra || "",
                             itemSubMenu: itemContent[i].itemSubMenu,
                             isActive: itemContent[i].isActive,
                             checked: itemContent[i].checked
                         })
        }
    }

    Keys.onEscapePressed: Qt.quit()
    Keys.onLeftPressed: global_menu.parentMenu.requestFocus()
    Keys.onRightPressed: global_menu.childMenu.requestFocus()

    delegate: Rectangle {
        id: item
        state: isActive ? "normal" : "inactive"
        width: listview.width
        height: 24

        property bool isCheckable: itemId.split(":") != itemId
        property bool isActive: true
        property bool hasSubMenu: itemSubMenu && itemSubMenu != "[]"

        states: [
            State {
                name: "normal"
                PropertyChanges {
                    target: item
                    color: "transparent"
                }
                PropertyChanges {
                    target: menu_item_text
                    color: listview.itemTextColor
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
                    target: item
                    color: listview.itemHoverColor
                }
                PropertyChanges {
                    target: menu_item_text
                    color: listview.itemTextHoverColor
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
                name: "pressed"
                PropertyChanges {
                    target: item
                    color: listview.itemPressedColor
                }
                PropertyChanges {
                    target: menu_item_text
                    color: listview.itemTextPressedColor
                }
                PropertyChanges {
                    target: icon_image
                    source: item.isCheckable ? listview.checkmarkPressed : itemIconPressed
                }
                PropertyChanges {
                    target: sub_menu_indicator
                    source: item.hasSubMenu ? listview.arrowPressed : ""
                }
            },
            State {
                name: "inactive"
                PropertyChanges {
                    target: item
                    color: "transparent"
                }
                PropertyChanges {
                    target: menu_item_text
                    color: listview.itemTextInactiveColor
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
                    item.state = item.isActive ? "hover" : "inactive"
                    item.hasSubMenu && item.showSubMenu()
                } else if (listview.currentIndex != -1) {
                    item.state = item.isActive ? "normal" : "inactive"
                    global_menu.destroySubMenu()
                }
            }
        }

        function showSubMenu() {
            global_menu.destroySubMenu()

            var itemGlobalPos = item.mapToItem(global_screen, 0, 0)
            global_menu.showSubMenu(itemGlobalPos.x + item.width,
                                    itemGlobalPos.y, itemSubMenu)
        }

        Image {
            id: icon_image
            width: 14
            height: 14

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
            onPressed: parent.state = parent.isActive ? "pressed" : "inactive"
            onReleased: parent.state = parent.isActive ? "hover" : "inactive"
        }
    }

    model: ListModel {
    }
}
