import QtQuick 2.0

import "MenuItem.js" as MenuItemJs

Rectangle {
    id: fullscreen_bg
	/* focus: true */
    width: _injection.getScreenWidth()
    height: _injection.getScreenHeight()
    color: Qt.rgba(1, 1, 1, 1)

    MouseArea {
        id: mouseArea
        anchors.fill: parent
		acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
			/* mouse.accepted = false */
			/* _injection.postMouseEvent(mouse.x, mouse.y, 1) */
            _application.quit()
        }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Escape) {
            _application.quit()
        }
    }
	
	Component.onCompleted: {
		var component
		if (_menu_view.withCorner) {
			component = Qt.createComponent("DockMenu.qml")
		} else {
			component = Qt.createComponent("RectMenu.qml")
		}
        component.createObject(fullscreen_bg, {"x": _menu_view.x, "y": _menu_view.y,
                                               "menuItems": _menu_view.menuJsonContent, "fullscreenBg": fullscreen_bg});
	}
}