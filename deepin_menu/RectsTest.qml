import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    width: 1000
    height: 1000

    RectWithCorner {
        x: 200
        y: 200
		rectWidth: 200
		rectHeight: 300 
    }
	
	Rect {
		x: 500
		y: 200
		rectWidth: 200
		rectHeight: 300
	}
}