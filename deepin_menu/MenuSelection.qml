import QtQuick 2.1

Component {
	id: component
	
	Rectangle {
		Rectangle {
			id: topShader
			
			width: parent.width
			height: 1
			color: "#3ca4e6"
			
			anchors.top: parent.top
		}
		
		Rectangle {
			width: parent.width
			height: parent.height - 2
			
			gradient: Gradient {
                GradientStop { position: 0.0; color: "#43b6ff" }
                GradientStop { position: 0.1; color: "#2aadff" }
			}
			
			anchors.top: topShader.bottom
			anchors.bottom: bottomShader.top
		}
		
		Rectangle {
			id: bottomShader
			
			width: parent.width
			height: 1
			color: "#269ae6"
			
			anchors.bottom: parent.bottom
		}
	}
}