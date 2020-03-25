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
        anchors.rightMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        placeholderText: qsTr("http(s)://")
    }

    Button {
        id: button
        y: 30
        text: qsTr("Скачать")
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: textField.verticalCenter
        onClicked: log.text = ""
    }

    Connections {
        target: button
        onClicked: backend.download(textField.text.toString());
    }

    TextArea {
        objectName: "log"
        id: log
        height: 243
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        readOnly: true
        font.family: "Courier"
        font.pointSize: 12
        textFormat: TextEdit.RichText
        onLinkActivated: Qt.openUrlExternally(log.hoveredLink)
    }

    ListView {
        id: listProgress
        anchors.bottom: log.top
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: textField.bottom
        anchors.topMargin: 20
        model: myModel
        delegate: Row {
            spacing: 5
            Text {
                text: page
                width: 300
                elide: Text.ElideLeft
//                wrapMode: Text.WrapAnywhere
                font.pointSize: 12
//                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
            }
            ProgressBar{
                value: percentage
                minimumValue: 0
                maximumValue: 100
            }
            Text {
                text: percentage + " %"
                font.pointSize: 12
            }
        }
    }

}
