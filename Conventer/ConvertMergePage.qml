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
                if (drop.hasUrls) {
                    var urls = drop.urls
                    if (urls && urls.length > 0) addFilesFromUrls(urls)
                }
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
                        width: parent.width - 300
                        Text { elide: Text.ElideRight; text: displayName }
                        Text { color: "#666"; text: type + (status ? (" • " + status) : "") }
                    }
                    Row {
                        spacing: 4
                        Column {
                            spacing: 6
                            Button { text: "↑"; onClicked: moveUp(index) }
                            Button { text: "↓"; onClicked: moveDown(index) }
                        }
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



    function addFilesFromUrls(urls) {
        for (var i = 0; i < urls.length; ++i) {
            var u = urls[i]
            var local = toLocalPath(u)
            var type = fileType(local)

            if (type === "other") {
                continue
            }

            var display = getDisplayName(local)
            var preview = ""
            fileModel.append({
                                 "path": local,
                                 "displayName": display,
                                 "type": type,
                                 "preview": preview,
                                 "status": ""
                             })

            if (type === "pdf") pdfRenderQueue.push(local)
            else if (type === "image") {
                var itemIndex = fileModel.count - 1
                fileModel.set(itemIndex, {
                                  "path": local,
                                  "displayName": display,
                                  "type": type,
                                  "preview": fileUrl(local),
                                  "status": ""
                              })
            }
        }
        if (!isRenderingPdf) renderNextPdf()
    }


    function renderNextPdf() {
        if (pdfRenderQueue.length === 0) {
            isRenderingPdf = false
            return
        }
        if (isRenderingPdf) return
        isRenderingPdf = true
        currentRenderingPath = pdfRenderQueue.shift()
        appController.renderPdfPageToTempImage(currentRenderingPath, 1)
    }

    Connections {
        target: appController
        function onPreviewReady(tempPreview) {
            for (var i = 0; i < fileModel.count; ++i) {
                var item = fileModel.get(i)
                if (item.path === currentRenderingPath) {
                    fileModel.set(i, {
                                      "path": item.path,
                                      "displayName": item.displayName,
                                      "type": item.type,
                                      "preview": tempPreview,
                                      "status": item.status
                                  })
                    break
                }
            }
            isRenderingPdf = false
            renderNextPdf()
        }
        function onProgressUpdated(value) { progressBar.value = value }
        function onStatusUpdated(message) { statusLabel.text = message }
        function onFilesMerged(success) {
            if (success) {
                statusLabel.text = "Готово: объединение завершено"
                lastOutputPath = pendingOutputTemp
                openFolderBtn.enabled = true
            } else statusLabel.text = "Ошибка при объединении"
        }
        function onImagesConverted(success) {
            if (success) {
                statusLabel.text = "Готово: конвертация изображений завершена"
                openFolderBtn.enabled = true
            } else statusLabel.text = "Ошибка при конвертации изображений"
        }
        function onDocConverted(success) {
            statusLabel.text = success ? "Готово: DOC сконвертирован" : "Ошибка при конвертации DOC"
        }
    }

    function toLocalPath(url) {
        if (!url) return ""
        var s = (typeof url === "string") ? url : url.toString()
        if (s.indexOf("file:///") === 0) return s.replace("file:///", "")
        if (s.indexOf("file://") === 0) return s.replace("file://", "")
        if (s.indexOf("file:") === 0) return s.replace("file:", "")
        return s
    }

    function fileUrl(path) {
        if (!path) return ""
        var p = path.toString().replace(/\\/g, "/")
        if (p.indexOf("file:///") === 0) return p
        return "file:///" + p
    }

    function getDisplayName(path) {
        var s = path.replace(/\\/g, "/")
        var idx = s.lastIndexOf("/")
        if (idx >= 0) return s.substring(idx + 1)
        return s
    }

    function fileType(path) {
        var ext = path.split(".").pop().toLowerCase()
        if (["png","jpg","jpeg"].indexOf(ext) !== -1) return "image"
        if (ext === "pdf") return "pdf"
        if (ext === "doc" || ext === "docx") return "doc"
        return "other"
    }

    function getOutputFolder(path) {
        if (!path) return ""
        var p = path.replace("file:///", "").replace(/\\/g, "/")
        if (p.match(/\.pdf$/i)) {
            var idx = p.lastIndexOf("/")
            if (idx >= 0) return p.substr(0, idx)
            return p
        }
        return p
    }

    function getPathsInOrder() {
        var arr = []
        for (var i = 0; i < fileModel.count; ++i)
            arr.push(fileModel.get(i).path)
        return arr
    }


    function removeAt(idx) { fileModel.remove(idx) }

    function moveUp(idx) {
        if (idx <= 0) return
        fileModel.move(idx, idx - 1, 1)
    }

    function moveDown(idx) {
        if (idx >= fileModel.count - 1) return
        fileModel.move(idx, idx + 1, 1)
    }

    function startProcessing() {
        var paths = getPathsInOrder()
        if (paths.length === 0) { statusLabel.text = "Список файлов пуст"; return }
        lastStartedPaths = paths
        if (mergeCheckbox.checked) saveDialog.open()
        else folderDialog.open()
    }

    property string pendingOutputTemp: ""
    function startProcessingWithPath(outPath) {
        pendingOutputTemp = outPath
        lastOutputPath = outPath
        var paths = getPathsInOrder()
        if (mergeCheckbox.checked) appController.addFilesToQueue(paths, true, outPath)
        else appController.addFilesToQueue(paths, false, outPath)
        statusLabel.text = "Запущено..."
    }

}
