import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root
    width: holder.width
    height: holder.height

    property int leftMargin: 0
    property int rightMargin: 0
    property int topMargin: 5
    property int bottomMargin: 5

    property alias glowRadius: shadow_effect.radius

    default property alias content: content_item.children

    Item {
        id: holder
        width: bg_rect.width + shadow_effect.radius * 2
        height: bg_rect.height + shadow_effect.radius * 2

        Rectangle {
            id: bg_rect
            radius: 4
            color: "white"
            width: content_item.width + root.leftMargin + root.rightMargin
            height: content_item.height + root.topMargin + root.bottomMargin
            anchors.centerIn: parent
        }
    }

    DropShadow {
        id: shadow_effect

        anchors.fill: holder
        horizontalOffset: 0
        verticalOffset: 3
        radius: 16
        samples: 16
        color: Qt.rgba(0, 0, 0, 0.4)
        source: holder
    }

    Item {
        id: content_item
        width: childrenRect.width
        height: childrenRect.height

        anchors.left: holder.left
        anchors.leftMargin: shadow_effect.radius + root.leftMargin
        anchors.top: holder.top
        anchors.topMargin: shadow_effect.radius + root.topMargin
    }
}
