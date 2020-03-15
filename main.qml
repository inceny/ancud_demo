import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Demo")

    TextField {
        id: textField
        height: 20
        text: "https://yandex.ru"
        anchors.right: parent.right
        anchors.rightMargin: 200
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 30
        placeholderText: qsTr("http(s)://")
    }

    Button {
        id: button
        y: 30
        text: qsTr("Скачать")
        anchors.left: textField.right
        anchors.leftMargin: 30
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: textField.verticalCenter
    }

    Connections {
        target: button
        onClicked: backend.download(textField.text.toString());
    }

    TextArea {
        objectName: "log"
        id: log
        readOnly: true
        font.family: "Courier"
        font.pointSize: 12
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: textField.bottom
        anchors.topMargin: 30
    }
}

/*##^##
Designer {
    D{i:4;anchors_height:178;anchors_width:292;anchors_x:115;anchors_y:185}
}
##^##*/
