import QtQuick 2.0
import DBus.Com.Deepin.Daemon.Display 1.0
import "MenuItem.js" as MenuItemJs

ListView {
    id: listview
    objectName: "listview"
    currentIndex: -1
    model: MenuItemListModel { menuJsonContent: listview.menuJsonContent}
    delegate: MenuItemDelegate {}
    /* keyNavigationWraps: true */
    interactive: false

    property int textSize: 12
    property int textLeftMargin: 22
    property int textRightMargin: 22
    property int horizontalPadding: 3
    property int verticalPadding: 3
    property int pictureSize: 16
    property int minWidth: 150

    property string arrowDark: "images/arrow-dark.png"
    property string arrowDarkHover: "images/arrow-dark-hover.png"
    property string arrowLight: "images/arrow-light.png"
    property string arrowLightHover: "images/arrow-light-hover.png"

    property color textColor: Qt.rgba(1, 1, 1, 1)
    property color textColorHover: Qt.rgba(0, 0, 0, 1)
    property color textColorNotActive: Qt.rgba(0.5, 0.5, 0.5, 1)
    property color extraColor: "#636363"

    property string menuJsonContent: ""

    property bool isDockMenu: false
    property bool isCheckableMenu: false
    property bool isSingleCheck: false

    Display { id: dbus_display }
    Monitor { id: dbus_monitor }

    property rect currentMonitorRect: {
        var monitors = dbus_display.monitors
        for (var i = 0; i < monitors.length; i++) {
            dbus_monitor.path = monitors[i]
            if (dbus_monitor.x <= _menu_view.x &&
                _menu_view.x <= dbus_monitor.x + dbus_monitor.width &&
                dbus_monitor.y <= _menu_view.y &&
                _menu_view.y <= dbus_monitor.y + dbus_monitor.height) {

                return Qt.rect(dbus_monitor.x, dbus_monitor.y,
                               dbus_monitor.width,
                               dbus_monitor.height)
            }
        }
    }

    signal itemChecked(int idx, var item)
    signal itemUnchecked(int idx, var item)
    signal selectItemPrivate(int idx)

    onItemChecked: {
        _menu_view.notifyUpdateItemChecked(item.componentId, true)
    }

    onItemUnchecked: {
        _menu_view.notifyUpdateItemChecked(item.componentId, false)
    }

    Timer {
        id: timer
        interval: 250
        running: false
        repeat: false

        property string jsonContent
        onTriggered: _menu_view.showSubMenu(jsonContent)
    }

    /* below two property is created for onCurrentItemChanged method. */
    property int lastCurrentIndex: 0
    property var lastCurrentItem: null
    onCurrentItemChanged: {
        if (currentItem.isSep || !currentItem.componentActive) {
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
                lastCurrentItem.itemTextColor = lastCurrentItem.componentActive ? textColor : textColorNotActive
                lastCurrentItem.itemExtraColor = "#636363"
                lastCurrentItem.itemArrowPic = listview.arrowDark
                lastCurrentItem.itemIconPic = lastCurrentItem.iconNormal
            }

            /* selection effect */
            if (currentItem != null && currentItem.componentActive) {
                currentItem.itemTextColor = textColorHover
                currentItem.itemExtraColor = "white"
                currentItem.itemArrowPic = listview.arrowDarkHover
                currentItem.itemIconPic = currentItem.iconHover
            }
        } else {
            /* clear selection effect */
            if (lastCurrentItem != null) {
                lastCurrentItem.itemTextColor = lastCurrentItem.componentActive ? textColor : textColorNotActive
                lastCurrentItem.itemExtraColor = "#636363"
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
                currentItem.itemExtraColor = "white"                
                currentItem.itemArrowPic = listview.arrowLightHover
                currentItem.itemIconPic = currentItem.iconHover
            }
        }
        lastCurrentItem = currentItem

        // Create new subMenu
        if (hasSubMenu(currentItem.componentSubMenu)) {
            _menu_view.destroySubs()
            var component_menuJsonContent = currentItem.componentSubMenu

            var component_size = currentItem.ListView.view.getSize(component_menuJsonContent)
            var component_is_checkable_menu = currentItem.ListView.view.isCheckableMenuTest(component_menuJsonContent)
            var component_is_single_check = currentItem.ListView.view.isSingleCheckTest(component_menuJsonContent)
            var component_width = component_size.width
            var component_height = component_size.height

            var component_x = _menu_view.x + menu.width - menu.blurWidth * 2
            var component_y = _menu_view.y + y + currentItem.y - menu.blurWidth

            if (component_x + component_width> currentMonitorRect.x + currentMonitorRect.width) {
                component_x = _menu_view.x - component_width
            }

            if (component_y + component_height > currentMonitorRect.y + currentMonitorRect.height) {
                component_y = (currentMonitorRect.y + currentMonitorRect.height) - component_height
            }

            timer.jsonContent = JSON.stringify({"x": component_x, "y": component_y,
                                                "isDockMenu": menu.isDockMenu,
                                                "isCheckableMenu": component_is_checkable_menu,
                                                "isSingleCheck": component_is_single_check,
                                                "menuJsonContent": component_menuJsonContent})
            timer.restart()
        } else {
            timer.stop()
            _menu_view.destroySubs()
        }
    }

    function hasSubMenu(menuJsonContent) {
        if (menuJsonContent == null) {
            return false
        }

        var menu = JSON.parse(menuJsonContent)
        return menu.items.length != 0
    }

    function isCheckableMenuTest(menuJsonContent) {
        var menu = JSON.parse(menuJsonContent)
        return menu.checkableMenu
    }

    function isSingleCheckTest(menuJsonContent) {
        var menu = JSON.parse(menuJsonContent)
        return menu.singleCheck
    }

    function getNextItemsHasShortcut(startPos, keycode) {
        for (var i = Math.max(startPos, 0); i < listview.count; i++) {
            // a trick here, using currentIndex as the cursor.
            if (keycode == _injection.keyStringToCode(listview.model.get(i).itemShortcut)) {
                print(listview.model.get(i).itemText)
                return i
            }
        }

        for (var i = 0; i < listview.count; i++) {
            // we do the check another time to support wrapping.
            if (keycode == _injection.keyStringToCode(listview.model.get(i).itemShortcut)) {
                print(listview.model.get(i).itemText)
                return i
            }
        }
        return null
    }

    function getSize(menuJsonContent) {
        var menu = JSON.parse(menuJsonContent)
        var items = menu.items
        var _width = 0
        var _height = 0
        for (var i in items) {
            var menuTextWidth = _injection.getStringWidth(items[i].itemText.replace("_", ""), textSize)
            var extraTextWidth = _injection.getStringWidth(items[i].itemExtra || "", textSize)
            var itemWidth = items[i].itemExtra ?  Math.max(menuTextWidth + textLeftMargin + textRightMargin + horizontalPadding, 100) + extraTextWidth + 30 : menuTextWidth + textLeftMargin + textRightMargin + horizontalPadding
            
            if (itemWidth > _width) {
                _width = itemWidth
            }

            if (items[i].itemText == undefined || items[i].itemText == "") {
                _height += verticalPadding * 2 + 2
            } else {
                _height += _injection.getStringHeight(items[i].itemText, textSize) + verticalPadding * 2
            }
        }
        return {"width": Math.max(_width, minWidth), "height": _height}
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

    function updateCheckableItem(id, value) {
        listview.model.updateItemChecked(id, value)
    }

    function updateItemActivity(id, value) {
        listview.model.updateItemActivity(id, value)
    }

    function updateItemText(id, value) {
        listview.model.updateItemText(id, value)
    }

    Component.onCompleted: {
        var size = getSize(menuJsonContent)

        listview.width = size.width
        listview.height = size.height

        if (!isDockMenu) {
            setHasSelection()
        }
    }

    Keys.onPressed: {
        switch(event.key) {

            case Qt.Key_Escape:
            _menu_view.destroyWholeMenu()
            break;

            case Qt.Key_Up:
            currentIndex = Math.max(currentIndex - 1, 0)
            break;

            case Qt.Key_Down:
            currentIndex = Math.min(currentIndex + 1, listview.count - 1)
            break;

            case Qt.Key_Left:
            _menu_view.activateParent()
            break;

            case Qt.Key_Right:
            _menu_view.activateSubMenu()
            break;

            case Qt.Key_Return:
            MenuItemJs.onClicked(currentIndex, currentItem, parent)
            break;

            default: {
                var _next_index_has_shortcut = getNextItemsHasShortcut(currentIndex + 1, event.key)
                if (_next_index_has_shortcut != null) {
                    listview.selectItemPrivate(_next_index_has_shortcut)
                    if (_next_index_has_shortcut == getNextItemsHasShortcut(_next_index_has_shortcut + 1, event.key)) {
                        MenuItemJs.onClicked(currentIndex, currentItem, parent)
                    }
                }
            }
        }
    }
}
