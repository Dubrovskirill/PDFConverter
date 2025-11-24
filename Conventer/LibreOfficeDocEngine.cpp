#include "LibreOfficeDocEngine.h"
#include <QProcess>
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

    QString outputDir = outputInfo.absolutePath();
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

    process.waitForFinished(30000); // 30 seconds timeout

    // LibreOffice создаёт PDF с именем входного файла → нужно переименовать
    QString generatedPdf =
        outputDir + "/" + inputInfo.completeBaseName() + ".pdf";

    if (!QFileInfo::exists(generatedPdf)) {
        qWarning() << "LibreOffice did not produce output PDF";
        return false;
    }

    // Переименовываем в то имя, которое запросил пользователь
    QFile::remove(outputPdfPath);
    QFile::rename(generatedPdf, outputPdfPath);

    return QFileInfo::exists(outputPdfPath);
}
