#include "AppController.h"
#include "StorageService.h"
#include "PdfEngine_Poppler.h"
#include "ImageService.h"
#include <QTemporaryFile>
#include <QFileInfo>
#include <QDir>
#include <QtConcurrent/QtConcurrent>
#include "ConvertImageToPdfCommand.h"
#include "ConvertDocToPdfCommand.h"
#include "MergeFilesCommand.h"
#include "ExtractPdfPageCommand.h"
#include <QDebug>


AppController::AppController(ConverterFacade* facade,
                             JobQueue* queue,
                             IPdfEngine* pdfEngine,
                             IDocEngine* docEngine,
                             StorageService* storage,
                             QObject* parent)
    : QObject(parent),
    m_facade(facade),
    m_queue(queue),
    m_pdfEngine(pdfEngine),
    m_docEngine(docEngine),
    m_storage(storage)
{
}


void AppController::convertImagesAsync(QStringList files, QString outPdf)
{
    if (!m_queue) {
        qWarning() << "JobQueue is null, cannot enqueue convertImages";
        emit imagesConverted(false);
        return;
    }

    auto* cmd = new ConvertImageToPdfCommand(files, outPdf);

    connect(cmd, &IConverterCommand::finished, this, [this](bool success){
        emit imagesConverted(success);
    });
    connect(cmd, &IConverterCommand::progressChanged, this, [this](int v){
        emit progressUpdated(v);
    });
    connect(cmd, &IConverterCommand::statusChanged, this, [this](QString m){
        emit statusUpdated(m);
    });

    m_queue->enqueue(cmd);
}

void AppController::convertDocAsync(QString doc, QString outPdf)
{
    if (!m_queue || !m_docEngine) {
        qWarning() << "JobQueue or IDocEngine is null, cannot enqueue convertDoc";
        emit docConverted(false);
        return;
    }

    auto* cmd = new ConvertDocToPdfCommand(m_docEngine, doc, outPdf);

    connect(cmd, &IConverterCommand::finished, this, [this](bool success){
        emit docConverted(success);
    });
    connect(cmd, &IConverterCommand::progressChanged, this, [this](int v){
        emit progressUpdated(v);
    });
    connect(cmd, &IConverterCommand::statusChanged, this, [this](QString m){
        emit statusUpdated(m);
    });

    m_queue->enqueue(cmd);
}

void AppController::mergeFilesAsync(QStringList files, QString outPdf)
{
    if (!m_queue || !m_pdfEngine) {
        qWarning() << "JobQueue or IPdfEngine is null, cannot enqueue mergeFiles";
        emit filesMerged(false);
        return;
    }

    auto* cmd = new MergeFilesCommand(files, outPdf, m_pdfEngine, m_docEngine);

    connect(cmd, &IConverterCommand::finished, this, [this](bool success){
        emit filesMerged(success);
    });
    connect(cmd, &IConverterCommand::progressChanged, this, [this](int v){
        emit progressUpdated(v);
    });
    connect(cmd, &IConverterCommand::statusChanged, this, [this](QString m){
        emit statusUpdated(m);
    });

    m_queue->enqueue(cmd);
}

void AppController::extractPageAsync(QString pdf, int page, QString outImg)
{
    if (!m_queue || !m_pdfEngine) {
        qWarning() << "JobQueue or IPdfEngine is null, cannot enqueue extractPage";
        emit pageExtracted(false);
        return;
    }

    auto* cmd = new ExtractPdfPageCommand(m_pdfEngine, pdf, page, outImg);

    connect(cmd, &IConverterCommand::finished, this, [this](bool success){
        emit pageExtracted(success);
    });
    connect(cmd, &IConverterCommand::progressChanged, this, [this](int v){
        emit progressUpdated(v);
    });
    connect(cmd, &IConverterCommand::statusChanged, this, [this](QString m){
        emit statusUpdated(m);
    });

    m_queue->enqueue(cmd);
}

void AppController::addFilesToQueue(QStringList files, bool merge, QString outPath)
{
    if (files.isEmpty()) {
        emit statusUpdated("Список файлов пуст");
        return;
    }

    if (merge) {
        QString out = outPath;
        if (out.isEmpty())
            out = QDir::currentPath() + "/merged.pdf";
        mergeFilesAsync(files, out);
        return;
    }

    QStringList images;
    QStringList docs;
    for (const QString &p : files) {
        QFileInfo fi(p);
        QString s = fi.suffix().toLower();
        if (s == "png" || s == "jpg" || s == "jpeg") images << p;
        else if (s == "doc" || s == "docx") docs << p;
    }

    for (const QString &img : images) {
        QFileInfo fi(img);
        QString outPdf = fi.absolutePath() + "/" + fi.completeBaseName() + ".pdf";
        convertImagesAsync(QStringList() << img, outPdf);
    }

    for (const QString &doc : docs) {
        QFileInfo fi(doc);
        QString outPdf = fi.absolutePath() + "/" + fi.completeBaseName() + ".pdf";
        convertDocAsync(doc, outPdf);
    }
}

void AppController::convertPdfToPng(QString pdfPath, int pageIndex, QString outImgPath)
{
    QString out = outImgPath;
    if (out.isEmpty()) {
        if (m_storage) out = m_storage->createTempFile("png");
        else {
            QTemporaryFile tmp(QDir::temp().filePath("preview_XXXXXX.png"));
            tmp.setAutoRemove(false);
            if (tmp.open()) { out = tmp.fileName(); tmp.close(); }
        }
    }

    extractPageAsync(pdfPath, pageIndex, out);
}

void AppController::renderPdfPageToTempImage(QString pdfPath, int pageIndex)
{
    QtConcurrent::run([this, pdfPath, pageIndex]() {
        PdfEngine_Poppler engine;
        if (!engine.openDocument(pdfPath)) { emit statusUpdated("Ошибка: не удалось открыть PDF"); return; }

        int totalPages = engine.pageCount();
        if (totalPages <= 0) { emit statusUpdated("PDF не содержит страниц"); return; }

        int idx = pageIndex - 1;
        if (idx < 0 || idx >= totalPages) { emit statusUpdated("Некорректный индекс страницы"); return; }

        QImage img = engine.renderPageToImage(idx, 1500);
        if (img.isNull()) { emit statusUpdated("Ошибка рендера страницы"); return; }

        QString out; // путь к временному изображению
        if (m_storage) out = m_storage->createTempFile("png");
        else {
            QTemporaryFile tmp(QDir::temp().filePath("preview_XXXXXX.png"));
            tmp.setAutoRemove(false);
            if (!tmp.open()) { emit statusUpdated("Ошибка создания временного файла"); return; }
            out = tmp.fileName(); tmp.close();
        }

        ImageService svc;
        if (!svc.saveImage(img, out, "PNG")) { emit statusUpdated("Ошибка сохранения превью"); return; }

        emit previewReady(out); // сообщаем QML, что превью доступно
    });
}

void AppController::cancelAllJobs()
{
    if (!m_queue) return;
    m_queue->cancelAll();
}




