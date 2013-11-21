import QtQuick 2.0

ListView {
    id: listview
    focus: true                 /* to enable key navigation */
    currentIndex: -1
    model: MenuItemListModel { menuItems: listview.menuItems}
    delegate: MenuItemDelegate {}
    /* keyNavigationWraps: true */
    /* interactive: false */

    property var fullscreenBg: null
    property int textSize: 12
    property int textLeftMargin: 22
    property int textRightMargin: 22
    property int horizontalPadding: 3
    property int verticalPadding: 3
    property int pictureSize: 16
    property int minWidth: 100

    property string arrowDark: "images/arrow-dark.png"
    property string arrowDarkHover: "images/arrow-dark-hover.png"
    property string arrowLight: "images/arrow-light.png"
    property string arrowLightHover: "images/arrow-light-hover.png"

    property color textColor: Qt.rgba(1, 1, 1, 1)
    property color textColorHover: Qt.rgba(0, 0, 0, 1)
    property color textColorNotActive: Qt.rgba(0.5, 0.5, 0.5, 1)

    property string menuItems: ""

    property bool isDockMenu: false
	property bool isCheckableMenu: false
	property bool isSingleCheck: false
	
	signal itemChecked(int idx)

    /* below two property is created for onCurrentItemChanged method. */
    property int lastCurrentIndex: 0
    property var lastCurrentItem: null
    onCurrentItemChanged: {
        /* console.log(currentItem.isSep) */
        if (currentItem.isSep) {
            if (currentIndex > lastCurrentIndex) {
                currentIndex += 1
            }
            if (currentIndex < lastCurrentIndex) {
                currentIndex -= 1
            }
            lastCurrentIndex = currentIndex
        }

        if (isDockMenu) {
            /* clear selection effect */
            if (lastCurrentItem != null) {
                lastCurrentItem.itemTextColor = lastCurrentItem.componentActive ? textColor : textColorNotActive
                lastCurrentItem.itemArrowPic = listview.arrowDark
                lastCurrentItem.itemIconPic = lastCurrentItem.iconNormal
            }

            /* selection effect */
            if (currentItem != null && currentItem.componentActive) {
                currentItem.itemTextColor = textColorHover
                currentItem.itemArrowPic = listview.arrowDarkHover
                currentItem.itemIconPic = currentItem.iconHover
            }
        } else {
            /* clear selection effect */
            if (lastCurrentItem != null) {
                lastCurrentItem.itemTextColor = lastCurrentItem.componentActive ? textColor : textColorNotActive
                lastCurrentItem.itemArrowPic = listview.arrowLight
                lastCurrentItem.itemIconPic = lastCurrentItem.iconNormal
            }

            /* selection effect */
            if (!currentItem.componentActive) {
                setNoSelection()
            } else {
                setHasSelection()
            }
            if (currentItem != null && currentItem.componentActive) {
                currentItem.itemTextColor = textColorHover
                currentItem.itemArrowPic = listview.arrowLightHover
                currentItem.itemIconPic = currentItem.iconHover
            }

        }
        lastCurrentItem = currentItem

        // Destroy old subMenu
        if (menu.subMenuObj != null) {
            menu.subMenuObj.destroy(10)
            menu.subMenuObj = null
        }

        // Create new subMenu
        if (hasSubMenu(currentItem.componentSubMenu)) {
            if (currentItem.componentSubMenu != null) {
                var component = Qt.createComponent("RectMenu.qml");
                var component_menuItems = currentItem.componentSubMenu

                var component_size = currentItem.ListView.view.getSize(component_menuItems)
                var component_width = component_size.width
                var component_height = component_size.height
				var component_is_checkable_menu = currentItem.ListView.view.isCheckableMenuTest(component_menuItems)
				var component_is_single_check = currentItem.ListView.view.isSingleCheckTest(component_menuItems)

                var component_x = menu.x + menu.width - menu.blurWidth * 2
                var component_y = menu.y + currentItem.y

                if (component_x + component_width> fullscreen_bg.width) {
                    component_x = menu.x - component_width
                }

                if (component_y + component_height > fullscreen_bg.height) {
                    component_y = fullscreen_bg.height - component_height
                }

                var obj = component.createObject(fullscreen_bg, {"x": component_x, "y": component_y,
                                                                 "borderColor": menu.borderColor,
                                                                 "blurWidth": menu.blurWidth,
                                                                 "fillColor": menu.fillColor,
                                                                 "fontColor": menu.fontColor,
                                                                 "fontColorHover": menu.fontColorHover,
                                                                 "fontColorNotActive": menu.fontColorNotActive,
                                                                 "isDockMenu": menu.isDockMenu,
																 "isCheckableMenu": component_is_checkable_menu,
																 "isSingleCheck": component_is_single_check,
                                                                 "menuItems": component_menuItems, "fullscreenBg": fullscreenBg});
                menu.subMenuObj = obj
            }
        }
    }
	
	function hasSubMenu(menuItems) {
		var menu = JSON.parse(menuItems)
		return menu.items.length != 0
	}
	
	function isCheckableMenuTest(menuItems) {
        var menu = JSON.parse(menuItems)		
		return menu.checkableMenu
	}
	
	function isSingleCheckTest(menuItems) {
        var menu = JSON.parse(menuItems)		
		return menu.singleCheck
	}

    function getSize(menuItems) {
        var menu = JSON.parse(menuItems)
		var items = menu.items
        var _width = 0
        var _height = 0
        for (var i in items) {
            if (_injection.getStringWidth(items[i].itemText, textSize)
                + textLeftMargin + textRightMargin + horizontalPadding > _width) {
                _width = _injection.getStringWidth(items[i].itemText, textSize)
                + textLeftMargin + textRightMargin + horizontalPadding
            }

            if (items[i].itemText == undefined || items[i].itemText == "") {
                _height += verticalPadding * 2 + 2
            } else {
                _height += Math.max(_injection.getStringHeight(items[i].itemText, textSize) + verticalPadding * 2,
                                    pictureSize + verticalPadding * 2)
            }
        }
        return {"width": _width, "height": _height}
    }

    function setHasSelection() {
        var component = Qt.createComponent("MenuSelection.qml");
        listview.highlight = component.createObject(listview, {})
    }

    function setNoSelection() {
        var component = Qt.createQmlObject('import QtQuick 2.1; Component {Rectangle {visible:false}}',
                                           listview, "highlight");
        listview.highlight = component
    }

    Component.onCompleted: {
        var size = getSize(menuItems)

        listview.width = (Math.max(size.width, minWidth))
        listview.height = size.height

        if (!isDockMenu) {
            setHasSelection()
        }
    }
}
