import QtQuick 2.0

import "MenuItem.js" as MenuItemJs

Rectangle {
    id: fullscreen_bg
	focus: true
    width: _injection.getScreenWidth()
    height: _injection.getScreenHeight()
    color: Qt.rgba(0, 0, 0, 0)

    MouseArea {
        id: mouseArea
        anchors.fill: parent
		/* acceptedButtons: Qt.LeftButton | Qt.RightButton */
        onPressed: {
			/* mouse.accepted = false */
			/* _injection.postMouseEvent(mouse.x, mouse.y, 1) */
            /* _application.quit() */
        }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Escape) {
            _application.quit()
        }
    }

    Component.onCompleted: {
        var test_menu = '[{"itemIcon":"/usr/share/icons/Deepin/apps/16/preferences-driver.png","itemText":"Driver(<u>D</u>)","itemSubMenu":[{"itemIcon":"/usr/share/icons/Deepin/apps/16/preferences-display.png","itemText":"Display this this this this ", "itemSubMenu":[{"itemIcon":"/usr/share/icons/Deepin/apps/16/preferences-display.png","itemText":"Display this this this this ", "itemSubMenu":[]}]}]},{},{"itemIcon":"/usr/share/icons/Deepin/apps/16/preferences-display.png","itemText":"Display this this this this ", "itemSubMenu":[]}, {"itemIcon":"/usr/share/icons/Deepin/apps/16/preferences-driver.png","itemText":"Driver(<u>D</u>)","itemSubMenu":[{"itemIcon":"/usr/share/icons/Deepin/apps/16/preferences-display.png","itemText":"Display this this this this ", "itemSubMenu":[{"itemIcon":"/usr/share/icons/Deepin/apps/16/preferences-display.png","itemText":"Display this this this this ", "itemSubMenu":[]}]}]}]'
        var component = Qt.createComponent("ActualMenu.qml");
        var menu = component.createObject(fullscreen_bg, {"x": 400, "y": 100,
                                                          "width": 200,
                                                          "height": 400,
                                                          "menuItems": test_menu});
    }
}