#include "MergeFilesCommand.h"
#include <QDir>
MergeFilesCommand::MergeFilesCommand(const QStringList& inputFiles,
                                     const QString& outputPdf,
                                     IPdfEngine* pdfEngine,
                                     QObject* parent)
    : IConverterCommand(parent)
    , m_inputFiles(inputFiles)
    , m_outputPdf(outputPdf)
    , m_pdfEngine(pdfEngine)
{
}

bool MergeFilesCommand::execute()
{
    reportStatus("Начато объединение файлов в PDF");
    if(m_inputFiles.isEmpty()) {
        reportStatus("Список файлов пуст");
        emit finished(false);
        return false;
    }

    QStringList tempPdfs;
    ImageService imgService;

    int total = m_inputFiles.size();
    int index = 0;
    for (const QString& path : m_inputFiles)
    {
        if (m_cancelRequested) {
            reportStatus("Операция отменена");
            emit finished(false);
            return false;
        }

        QFileInfo info(path);
        QString suffix = info.suffix().toLower();

        reportStatus(QString("Обработка файла: %1").arg(path));

         if (suffix == "png" || suffix == "jpg" || suffix == "jpeg") {
            QImage img = imgService.loadImage(path);
            if (img.isNull()) {
                reportStatus(QString("Ошибка загрузки изображения: %1").arg(path));
                emit finished(false);
                return false;
            }
            img = imgService.normalizeImage(img);

            QString tempPdf = QDir::temp().filePath(info.baseName() + "_tmp.pdf");
            PdfWriter writer;
            if (!writer.beginDocument(tempPdf)) {
                reportStatus(QString("Ошибка создания временного PDF: %1").arg(tempPdf));
                emit finished(false);
                return false;
            }
            if (!writer.addImage(img)) {
                reportStatus(QString("Ошибка добавления изображения в временный PDF: %1").arg(path));
                emit finished(false);
                return false;
            }
            if (!writer.endDocument()) {
                reportStatus(QString("Ошибка сохранения временного PDF: %1").arg(tempPdf));
                emit finished(false);
                return false;
            }
             tempPdfs.append(tempPdf);
        } else if (suffix == "pdf") {
             tempPdfs.append(path);
        } else {
             reportStatus(QString("Файл пропущен (не PNG/JPG/PDF): %1").arg(path));
        }
        index++;
        reportProgress((index * 100) / total);
    }
    if (!m_pdfEngine->mergeDocuments(tempPdfs, m_outputPdf)) {
        reportStatus("Ошибка при объединении PDF");
        emit finished(false);
        return false;
    }
    reportStatus("Готово. PDF успешно создан.");
    emit finished(true);
    return true;

}

