import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    id: rect
    width: holder.width
    height: holder.height
    color: Qt.rgba(0, 0, 0, 0)

    property bool withBlur: true
    property color fillColor: Qt.rgba(0, 0, 0, 0.7)
    property color blurColor: Qt.rgba(0, 0, 0, 1)
    property color borderColor: Qt.rgba(1, 1, 1, 0.15)

    property int borderWidth: 2
    property int rectRadius: 4

    property int cornerPos: canvas.width / 2
    property int cornerWidth: 18
    property int cornerHeight: 10

    // properties intend to be exposed
    property int leftMargin: 0
    property int rightMargin: 0
    property int topMargin: 5
    property int bottomMargin: 5

    property alias glowRadius: shadow_effect.radius

    default property alias content: content_item.children

    onWidthChanged: canvas.requestPaint()
    onHeightChanged: canvas.requestPaint()
    onCornerPosChanged: canvas.requestPaint()

    Item {
        id: holder
        width: canvas.width + rect.glowRadius * 2
        height: canvas.height + rect.glowRadius * 2

        Canvas {
            id: canvas
            width: content_item.width + rect.leftMargin + rect.rightMargin
            height: content_item.height + rect.topMargin + rect.bottomMargin + rect.cornerHeight
            anchors.centerIn: parent

            onPaint: {
                var ctx = getContext("2d")

                var x = 2
                var y = 2
                var w = width - 3
                var h = height - 3

                ctx.save()
                ctx.clearRect(0, 0, canvas.width, canvas.height)

                ctx.beginPath();

                ctx.moveTo(x + rect.rectRadius, y);                 // top side
                ctx.lineTo(x + w - rect.rectRadius, y);
                // draw top right corner
                ctx.arcTo(x + w, y, x + w, y + rect.rectRadius, rect.rectRadius);
                ctx.lineTo(x + w, y + h - rect.cornerHeight - rect.rectRadius);    // right side
                // draw bottom right corner
                ctx.arcTo(x + w, y + h - rect.cornerHeight, x + w - rect.rectRadius, y + h - rect.cornerHeight, rect.rectRadius);

                rect.cornerPos = Math.max(rect.cornerPos, rect.rectRadius + rect.cornerWidth / 2)
                rect.cornerPos = Math.min(rect.cornerPos, width - rect.rectRadius - rect.cornerWidth / 2)

                ctx.lineTo(rect.cornerPos + rect.cornerWidth / 2, y + h - rect.cornerHeight) /* corner */
                ctx.lineTo(rect.cornerPos, y + h)
                ctx.lineTo(rect.cornerPos - rect.cornerWidth / 2, y + h - rect.cornerHeight)

                ctx.lineTo(x + rect.rectRadius, y + h - rect.cornerHeight);              // bottom side
                // draw bottom left corner
                ctx.arcTo(x, y + h - rect.cornerHeight, x, y + h - rect.rectRadius - rect.cornerHeight, rect.rectRadius);
                ctx.lineTo(x, y + rect.rectRadius);                 // left side
                // draw top left corner
                ctx.arcTo(x, y, x + rect.rectRadius, y, rect.rectRadius);

                ctx.closePath();

                ctx.lineWidth = borderWidth
                ctx.strokeStyle = borderColor
                ctx.stroke()

                var gradient = ctx.createLinearGradient(width / 2, 0, width / 2, height);
                gradient.addColorStop(0.0, Qt.rgba(0, 0, 0, 0.55));
                gradient.addColorStop(1.0, Qt.rgba(0, 0, 0, 0.65));
                ctx.fillStyle = gradient
                ctx.fill()

                ctx.restore()
            }
        }
    }

    DropShadow {
        id: shadow_effect
        anchors.fill: holder
        visible: rect.withBlur
        horizontalOffset: 0
        verticalOffset: 3
        radius: 10
        samples: 16
        color: rect.blurColor
        source: holder
    }

    Item {
        id: content_item
        width: childrenRect.width
        height: childrenRect.height

        anchors.left: holder.left
        anchors.leftMargin: shadow_effect.radius + rect.leftMargin
        anchors.top: holder.top
        anchors.topMargin: shadow_effect.radius + rect.topMargin
    }
}
