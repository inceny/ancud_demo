import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    TextField {
        id: textField
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 200
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 30
        placeholderText: qsTr("Text Field")
    }

    Button {
        id: button
        y: 30
        text: qsTr("Button")
        anchors.left: textField.right
        anchors.leftMargin: 30
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: textField.verticalCenter
    }

    Connections {
        target: button
        onClicked: backend.download(textField.text.toString());
    }
}
