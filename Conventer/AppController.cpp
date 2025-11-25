#include "AppController.h"
#include "ConvertImageToPdfCommand.h"
#include "ConvertDocToPdfCommand.h"
#include "MergeFilesCommand.h"
#include "ExtractPdfPageCommand.h"
#include <QDebug>


AppController::AppController(ConverterFacade* facade,
                             JobQueue* queue,
                             IPdfEngine* pdfEngine,
                             IDocEngine* docEngine,
                             QObject* parent)
    : QObject(parent),
    m_facade(facade),
    m_queue(queue),
    m_pdfEngine(pdfEngine),
    m_docEngine(docEngine)
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


