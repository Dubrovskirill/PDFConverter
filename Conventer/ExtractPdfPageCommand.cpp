#include "ExtractPdfPageCommand.h"


#include <QDebug>

ExtractPdfPageCommand::ExtractPdfPageCommand(IPdfEngine* engine,
                                             const QString& pdfPath,
                                             int pageIndex,
                                             const QString& outputImage,
                                             QObject* parent)
    : IConverterCommand(parent)
    , m_engine(engine)
    , m_pdfPath(pdfPath)
    , m_pageIndex(pageIndex)
    , m_outputImage(outputImage)
{
}

bool ExtractPdfPageCommand::execute()
{
    reportStatus("Начало извлечения страницы из PDF");


    if (!m_engine) {
        reportStatus("Ошибка: IPdfEngine не задан");
        emit finished(false);
        return false;
    }


    if (!m_engine->openDocument(m_pdfPath)) {
        reportStatus("Ошибка: не удалось открыть PDF");
        emit finished(false);
        return false;
    }

    int totalPages = m_engine->pageCount();
    if (totalPages <= 0) {
        reportStatus("Ошибка: в PDF нет страниц");
        emit finished(false);
        return false;
    }


    int popplerIndex = m_pageIndex - 1;


    if (popplerIndex < 0 || popplerIndex >= totalPages) {
        reportStatus(QString("Ошибка: страницы %1 нет в документе (всего %2)")
                     .arg(m_pageIndex)
                     .arg(totalPages));
        emit finished(false);
        return false;
    }

    reportStatus(QString("Рендер страницы %1...").arg(m_pageIndex));


    QImage img = m_engine->renderPageToImage(popplerIndex, 1500);
    if (img.isNull()) {
        reportStatus("Ошибка: не удалось отрендерить страницу");
        emit finished(false);
        return false;
    }


    img = m_imageService.normalizeImage(img);


    if (!m_imageService.saveImage(img, m_outputImage, "PNG")) {
        reportStatus("Ошибка: не удалось сохранить изображение");
        emit finished(false);
        return false;
    }

    reportStatus("Страница успешно извлечена");
    emit finished(true);
    return true;
}

void ExtractPdfPageCommand::cancel()
{
    m_cancelRequested = true;
    reportStatus("Операция отменена");
}
