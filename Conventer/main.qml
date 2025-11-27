import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("PDF Converter")


    ToolBar {
        id: topBar
        anchors.top: parent.top
        width: parent.width

        Row {
            anchors.fill: parent
            spacing: 12

            Button {
                id: backButton
                text: "< Назад"
                visible: stack.depth > 1
                onClicked: {
                    if (stack.depth > 1) stack.pop()
                }
            }

            Text {
                id: titleLabel
                text: root.title
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }


    StackView {
        id: stack
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        initialItem: mainPage
    }

    Component {
        id: mainPage
        Page {
            Column {
                anchors.centerIn: parent
                spacing: 20

                Rectangle {
                    width: 300; height: 80; color: "#3a7"; radius: 8
                    MouseArea { anchors.fill: parent; onClicked: stack.push("qrc:/qml/ConvertMergePage.qml") }
                    Text { anchors.centerIn: parent; text: "Конвертировать/объеденить в PDF" }
                }

                Rectangle {
                    width: 300; height: 80; color: "#37a"; radius: 8
                    MouseArea { anchors.fill: parent; onClicked: stack.push("qrc:/qml/ConvertPdfToPngPage.qml") }
                    Text { anchors.centerIn: parent; text: "Конвертировать из PDF в PNG" }
                }
            }
        }
    }
}
