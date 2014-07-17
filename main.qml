import QtQuick 2.2
import QtQuick.Window 2.1

Item {
    width: listview.width
    height: listview.height

    onWidthChanged: print(width)

    ListView {
        id: listview
        width: childrenRect.width
        height: childrenRect.height
        delegate: Text {
            text: itemText
        }
        model: ListModel {
            ListElement {
                itemText: "hello"
            }
            ListElement {
                itemText: "hello"
            }
            ListElement {
                itemText: "hellohellohellohellohellohellohellohello"
            }
            ListElement {
                itemText: "hello"
            }
            ListElement {
                itemText: "hello"
            }
        }

        Component.onCompleted: {
            model.append({"itemText": "hello"});
            model.append({"itemText": "hellohellohellohellohellohellohellohellohello"});
            model.append({"itemText": "hello"});
            model.append({"itemText": "hello"});
            model.append({"itemText": "hello"});
            model.append({"itemText": "hello"});
        }
    }
}
