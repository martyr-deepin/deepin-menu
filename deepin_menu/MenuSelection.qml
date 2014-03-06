import QtQuick 2.1

Component {
    id: component
    Rectangle {

        Rectangle {
            width: parent.width
            height: parent.height - 2

            gradient: Gradient {
                /* GradientStop { position: 0.0; color: "#43b6ff" } */
                /* GradientStop { position: 0.1; color: "#2aadff" } */
                GradientStop { position: 0.0; color: "#535353" }
                GradientStop { position: 0.1; color: "#323232" }
            }

            /* anchors.top: topShader.bottom */
            anchors.top: parent.top
            anchors.bottom: bottomShader.top

            Rectangle {
                id: topShader

                width: parent.width
                height: 1
                color: Qt.rgba(0, 0, 0, 0.3)
                
                anchors.top: parent.top
            }
        }

        Rectangle {
            id: bottomShader

            width: parent.width
            height: 1
            color: "#282828"

            anchors.bottom: parent.bottom
        }
    }
}