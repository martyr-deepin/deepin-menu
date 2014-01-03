import QtQuick 2.0
import "MenuItem.js" as MenuItemJs

ListView {
    id: listview
    focus: true                 /* to enable key navigation */
    currentIndex: -1
    model: MenuItemListModel { menuJsonContent: listview.menuJsonContent}
    delegate: MenuItemDelegate {}
    /* keyNavigationWraps: true */
    /* interactive: false */

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

    property string menuJsonContent: ""

    property bool isDockMenu: false
    property bool isCheckableMenu: false
    property bool isSingleCheck: false

    signal itemChecked(int idx, var item)
    signal itemUnchecked(int idx, var item)
    signal selectItemPrivate(string id)

    onItemChecked: {
        _menu_view.updateCheckableItem(item.componentId, true)
    }

    onItemUnchecked: {
        _menu_view.updateCheckableItem(item.componentId, false)
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

            if (component_x + component_width> _injection.getScreenWidth()) {
                component_x = _menu_view.x - component_width
            }

            if (component_y + component_height > _injection.getScreenHeight()) {
                component_y = _injection.getScreenHeight() - component_height
            }

            _menu_view.showSubMenu(JSON.stringify({"x": component_x, "y": component_y,
                                                   "isDockMenu": menu.isDockMenu,
                                                   "isCheckableMenu": component_is_checkable_menu,
                                                   "isSingleCheck": component_is_single_check,
                                                   "menuJsonContent": component_menuJsonContent}))
        } else {
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

    function getNextItemsHasShortcut(keycode) {
        for (var i = Math.max(currentIndex + 1, 0); i < listview.count; i++) {
            // a trick here, using currentIndex as the cursor.
            if (keycode == _injection.keyStringToCode(listview.model.get(i).itemShortcut)) {
                return listview.model.get(i).itemId
            }
        }

        for (var i = 0; i < listview.count; i++) {
            // we do the check another time to support wrapping.
            if (keycode == _injection.keyStringToCode(listview.model.get(i).itemShortcut)) {
                return listview.model.get(i).itemId
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
            if (_injection.getStringWidth(items[i].itemText.replace("_", ""), textSize)
                + textLeftMargin + textRightMargin + horizontalPadding > _width) {
                _width = _injection.getStringWidth(items[i].itemText.replace("_", ""), textSize)
                + textLeftMargin + textRightMargin + horizontalPadding
            }

            if (items[i].itemText == undefined || items[i].itemText == "") {
                _height += verticalPadding * 2 + 2
            } else {
                _height += Math.max(_injection.getStringHeight(items[i].itemText, textSize) + verticalPadding * 2,
                                    pictureSize + verticalPadding * 2)
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

            case Qt.Key_Left:
            _menu_view.activateParent()
            break;

            case Qt.Key_Right:
            _menu_view.activateSubMenu()
            break;

            case Qt.Key_Return:
            MenuItemJs.onPressed(currentIndex, currentItem, parent)
            break;

            default:
            if (getNextItemsHasShortcut(event.key)) { selectItemPrivate(getNextItemsHasShortcut(event.key)) }
        }
    }
}
