import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Qt.labs.platform 1.1

Page {
    id: convertMergePage
    title: "Конвертировать/объеденить в PDF"

    property var fileModel: ListModel { }
    property var pdfRenderQueue: []
    property bool isRenderingPdf: false
    property string currentRenderingPath: ""
    property string pendingOutputFolder: ""
    property var lastStartedPaths: []
    property string lastOutputPath: ""

    readonly property string iconPdf: "qrc:/icons/pdf_icon.png"
    readonly property string iconDoc: "qrc:/icons/doc_icon.png"
    readonly property string iconImage: "qrc:/icons/image_icon.png"

    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8


        Row { spacing: 8 }
        Row { spacing: 8 }
            Button {
                text: "Добавить файлы"
                onClicked: fileDialog.open()
            }
            CheckBox {
                id: mergeCheckbox
                text: "Объединить"
            }
            Button {
                text: "Запустить"
                onClicked: startProcessing()
            }
            Button {
                text: "Отмена"
                onClicked: appController.cancelAllJobs()
            }
            Button {
                text: "Очистить список"
                onClicked: { fileModel.clear(); pdfRenderQueue = [] }
            }


        DropArea {
            id: dropArea
            anchors.left: parent.left
            anchors.right: parent.right
            height: 60
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "lightgray"
                radius: 6
                Text { anchors.centerIn: parent; text: "Перетащите файлы сюда или используйте 'Добавить файлы'" }
            }
            onDropped: {
                var urls = drag.source.mimeData.urls
                if (urls && urls.length > 0) addFilesFromUrls(urls)
            }
        }


        ListView {
            id: filesList
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.55
            model: fileModel
            delegate: Rectangle {
                width: parent.width
                height: 90
                color: index % 2 === 0 ? "#fafafa" : "#ffffff"
                Row { anchors.fill: parent; spacing: 8; anchors.margins: 8
                    Image {
                        id: thumb
                        width: 64; height: 64
                        fillMode: Image.PreserveAspectFit

                        source: preview !== "" ? fileUrl(preview) : (type === "image" ? fileUrl(path) : (type === "pdf" ? iconPdf : iconDoc))
                    }
                    Column {
                        width: parent.width - 260
                        Text { elide: Text.ElideRight; text: displayName }
                        Text { color: "#666"; text: type + (status ? (" • " + status) : "") }
                    }
                    Column { spacing: 6
                        Button { text: "↑"; onClicked: moveUp(index) }
                        Button { text: "↓"; onClicked: moveDown(index) }
                        Button { text: "Удалить"; onClicked: removeAt(index) }
                    }
                }
            }
        }


        Row {
            spacing: 8
            ProgressBar { id: progressBar; width: parent.width * 0.7; from: 0; to: 100 }
            Text { id: statusLabel; text: "" }
            Button {
                id: openFolderBtn
                text: "Открыть папку"
                enabled: lastOutputPath !== ""
                onClicked: {
                    var folder = getOutputFolder(lastOutputPath)
                    if (folder !== "") Qt.openUrlExternally("file:///" + folder.replace(/\\/g, "/"))
                }
            }
        }
    }


    FileDialog {
        id: fileDialog
        title: "Выберите файлы"
        fileMode: FileDialog.OpenFiles
        onAccepted: {

            addFilesFromUrls(files)
        }
    }


    FileDialog {
        id: saveDialog
        title: "Сохранить объединённый PDF"
        fileMode: FileDialog.SaveFile
        nameFilters: ["PDF Files (*.pdf)"]
        onAccepted: {
            var outPath = toLocalPath(file)
            startProcessingWithPath(outPath)
        }
    }


    FolderDialog {
        id: folderDialog
        title: "Выберите папку для результатов"
        onAccepted: {
            var outFolder = toLocalPath(folder)
            startProcessingWithPath(outFolder)
        }
    }


}
