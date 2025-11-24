#include "ConvertImageToPdfCommand.h"

ConvertImageToPdfCommand::ConvertImageToPdfCommand(
    const QStringList& images,
    const QString& outputPdf,
    QObject* parent)
    : IConverterCommand(parent)
    , m_images(images)
    , m_outputPdf(outputPdf)
{
}

bool ConvertImageToPdfCommand::execute()
{
    reportStatus("Начато преобразование изображения в PDF");

    if(m_images.isEmpty()){
        reportStatus("Список изображений пуст");
        emit finished(false);
        return false;
    }

    PdfWriter writer;

    if (!writer.beginDocument(m_outputPdf)) {
        reportStatus("Ошибка: не удалось открыть PDF");
        emit finished(false);
        return false;
    }

    ImageService imgService;
    const int total = m_images.size();
    int index = 0;

    for (const QString& path: m_images)
    {
        if (m_cancelRequested) {
            reportStatus("Операция отменена");
            emit finished(false);
            return false;
        }
        reportStatus(QString("Загрузка изображения: %1").arg(path));
        QImage img = imgService.loadImage(path);

        if (img.isNull()) {
            reportStatus(QString("Ошибка загрузки: %1").arg(path));
            emit finished(false);
            return false;
        }
        img = imgService.normalizeImage(img);
        reportStatus(QString("Добавление в PDF: %1").arg(path));
        if (!writer.addImage(img)) {
            reportStatus("Ошибка добавления изображения в PDF");
            emit finished(false);
            return false;
        }
        index++;
        int progress = (index * 100) / total;
        reportProgress(progress);
    }
    if (!writer.endDocument()) {
        reportStatus("Ошибка сохранения PDF");
        emit finished(false);
        return false;
    }
    reportStatus("Готово. PDF успешно создан.");
    emit finished(true);
    return true;
}
