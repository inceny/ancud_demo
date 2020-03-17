import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.1

Window {
    id: window
    visible: true
    width: 640
    height: 480
    property alias buttonText: button.text
    title: qsTr("Demo")

    TextField {
        id: textField
        height: 25
        text: "https://yandex.ru"
        font.pointSize: 12
        anchors.right: button.left
        anchors.rightMargin: 30
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
        anchors.right: parent.right
        anchors.rightMargin: 30
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
        textFormat: TextEdit.RichText
    }

    MessageDialog{
        id: msgDialog
        objectName: "msgDialog"
        title: "Error"
        text: "Text"
        onAccepted: {
            Qt.quit()
        }
    }
}

/*##^##
Designer {
    D{i:4;anchors_height:178;anchors_width:292;anchors_x:115;anchors_y:185}
}
##^##*/
