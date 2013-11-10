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
        var component = Qt.createComponent("ActualMenu.qml");
		console.log(_menu_view.x)
		console.log(_menu_view.y)
		console.log(_menu_view.menuJsonContent)
        var menu = component.createObject(fullscreen_bg, {"x": _menu_view.x, "y": _menu_view.y,
                                                          "menuItems": _menu_view.menuJsonContent});
    }
}