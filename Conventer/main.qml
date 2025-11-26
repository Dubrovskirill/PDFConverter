import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("PDF Converter")

    StackView {
        id: stack
        anchors.fill: parent
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

