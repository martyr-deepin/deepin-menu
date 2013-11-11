import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    id: rect
    width: rectWidth
    height: rectHeight
	color: Qt.rgba(0, 0, 0, 0)

    property bool withBlur: true
    property color fillColor: Qt.rgba(0, 0, 0, 0.8)
    property color blurColor: Qt.rgba(0, 0, 0, 1)
    property color borderColor: Qt.rgba(1, 1, 1, 0.5)

    property int blurWidth: 5
    property int borderWidth: 2
    property int rectRadius: 4
    property int rectWidth: 100
    property int rectHeight: 100

    Canvas {
        id: canvas
        width: rectWidth
        height: rectHeight

        onPaint: {
            var ctx = getContext("2d")
            var x = blurWidth
            var y = blurWidth
            var w = rectWidth - 2 * blurWidth
            var h = rectHeight - 2 * blurWidth

            ctx.save()
            ctx.clearRect(0, 0, canvas.width, canvas.height)

            ctx.beginPath();
			ctx.roundedRect(x, y, w, h, rectRadius, rectRadius)
            ctx.closePath();

            ctx.lineWidth = borderWidth
            ctx.strokeStyle = borderColor
            ctx.stroke()
            ctx.fillStyle = fillColor
            ctx.fill()

            ctx.restore()
        }
    }
	
    Glow {
        anchors.fill: canvas
		visible: rect.withBlur
        radius: 10
        samples: 16
        color: rect.blurColor
        source: canvas
    }
}
