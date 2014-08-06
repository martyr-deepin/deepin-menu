import QtQuick 2.0

Item {
    Rectangle {
        id: topShader

        width: parent.width
        height: 1
        color: Qt.rgba(0, 0, 0, 0.3)

        anchors.top: parent.top
    }

    Rectangle {
        width: parent.width
        height: parent.height - 2

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#535353" }
            GradientStop { position: 0.1; color: "#323232" }
        }

        anchors.centerIn: parent
    }

    Rectangle {
        id: bottomShader

        width: parent.width
        height: 1
        color: "#282828"

        anchors.bottom: parent.bottom
    }
}
