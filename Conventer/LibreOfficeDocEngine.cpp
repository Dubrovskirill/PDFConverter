#include "LibreOfficeDocEngine.h"
#include <QProcess>
#include <QTemporaryDir>
#include <QFileInfo>
#include <QDebug>

LibreOfficeDocEngine::LibreOfficeDocEngine(QObject *parent)
    : QObject(parent)
{
}

QString LibreOfficeDocEngine::findLibreOfficeExecutable() const
{
#ifdef Q_OS_WIN
    // стандартный путь для Windows
    QString path1 = "C:/Program Files/LibreOffice/program/soffice.exe";
    QString path2 = "C:/Program Files (x86)/LibreOffice/program/soffice.exe";

    if (QFileInfo::exists(path1)) return path1;
    if (QFileInfo::exists(path2)) return path2;

    return "";
#else
    return "/usr/bin/soffice";   // Linux / Mac
#endif
}


bool LibreOfficeDocEngine::convertToPdf(const QString& inputPath,
                                        const QString& outputPdfPath)
{
    QString sofficePath = findLibreOfficeExecutable();
    if (sofficePath.isEmpty()) {
        qWarning() << "LibreOffice not found!";
        return false;
    }

    QFileInfo inputInfo(inputPath);
    QFileInfo outputInfo(outputPdfPath);

    // Создадим временную директорию для вывода LibreOffice, чтобы избежать мусора в целевом каталоге
    QTemporaryDir tmpOutDir; // tmp директория уничтожится в конце области видимости
    QString outputDir;
    if (tmpOutDir.isValid()) {
        outputDir = tmpOutDir.path(); // используем временную директорию
    } else {
        outputDir = outputInfo.absolutePath(); // fallback на директорию назначения
    }
    QString outputNameNoExt = outputInfo.completeBaseName();

    // Команда LibreOffice:
    // soffice --headless --convert-to pdf input.docx --outdir folder
    QStringList args;
    args << "--headless"
         << "--convert-to" << "pdf"
         << inputInfo.absoluteFilePath()
         << "--outdir" << outputDir;

    QProcess process;
    process.start(sofficePath, args);

    if (!process.waitForStarted(5000)) {
        qWarning() << "LibreOffice failed to start";
        return false;
    }

    if (!process.waitForFinished(30000)) { // 30 seconds timeout
        process.kill(); // убиваем процесс при таймауте
        process.waitForFinished(5000); // ждём завершения убийства
        qWarning() << "LibreOffice conversion timed out"; // сообщение об ошибке
        return false; // завершаем с ошибкой
    }

    // LibreOffice создаёт PDF с именем входного файла → нужно переименовать
    QString generatedPdf =
        outputDir + "/" + inputInfo.completeBaseName() + ".pdf";

    if (!QFileInfo::exists(generatedPdf)) {
        qWarning() << "LibreOffice did not produce output PDF";
        return false;
    }

    // Переименовываем файл в то имя, которое запросил пользователь
    QFile::remove(outputPdfPath);
    if (!QFile::rename(generatedPdf, outputPdfPath)) { // если rename не сработал
        // Попробуем скопировать в целевой путь и удалить исходный, иначе подчистим temp
        if (!QFile::copy(generatedPdf, outputPdfPath)) {
            qWarning() << "Failed to move generated PDF to target path" << generatedPdf << outputPdfPath; // сообщение об ошибке
            QFile::remove(generatedPdf); // удаляем остаточный файл
            return false;
        }
        QFile::remove(generatedPdf); // удалили временный файл после успешного копирования
    }

    // Если мы использовали temp-директорию, она будет автоматически удалена при выходе из области видимости
    return QFileInfo::exists(outputPdfPath); // возвращаем факт наличия целевого файла
}
