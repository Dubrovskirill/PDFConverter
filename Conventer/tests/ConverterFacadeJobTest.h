#ifndef CONVERTERFACADE_JOB_TEST_H
#define CONVERTERFACADE_JOB_TEST_H

#include <QtTest>
#include <QSignalSpy>

#include "ConverterFacade.h"
#include "PdfEngine_Poppler.h"
#include "LibreOfficeDocEngine.h"
#include "JobQueue.h"

#include "ConvertImageToPdfCommand.h"
#include "ConvertDocToPdfCommand.h"
#include "MergeFilesCommand.h"
#include "ExtractPdfPageCommand.h"

#include <poppler-qt5.h>

class ConverterFacadeJobTest : public QObject
{
    Q_OBJECT

private slots:
    void testFacadeAllOperationsAsync()
    {
        PdfEngine_Poppler pdfEngine;
        LibreOfficeDocEngine docEngine;

        ConverterFacade facade(&pdfEngine, &docEngine);
        JobQueue queue;

        // -----------------------------
        // 1. Convert images → PDF
        // -----------------------------
        QStringList images = {
                              "C:/Users/Кирилл/Downloads/3 этаж печать.jpg",
                              "C:/Users/Кирилл/Downloads/Лого 1.png",
                              "C:/Users/Кирилл/Downloads/олега режет.png",
                              "C:/Users/Кирилл/OneDrive/Pictures/Roblox/RobloxScreenShot20251019_103049833.png",
                              "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/ZMCptmrnlA.png"
        };

        QString outImagesPdf = QDir::currentPath() + "/facade_images_async.pdf";

        auto* cmd1 = new ConvertImageToPdfCommand(images, outImagesPdf);
        QSignalSpy spy1(cmd1, SIGNAL(finished(bool)));

        queue.enqueue(cmd1);

        QVERIFY(spy1.wait(60000));              // ждём завершения
        QVERIFY(spy1.takeFirst().at(0).toBool());
        QVERIFY(QFile::exists(outImagesPdf));

        // -----------------------------
        // 2. Convert DOC → PDF
        // -----------------------------
        QString docFile = "D:/Учеба/курсовая/Идент (1).docx";
        QString outDocPdf = QDir::currentPath() + "/facade_doc_async.pdf";

        auto* cmd2 = new ConvertDocToPdfCommand(&docEngine, docFile, outDocPdf);
        QSignalSpy spy2(cmd2, SIGNAL(finished(bool)));

        queue.enqueue(cmd2);

        QVERIFY(spy2.wait(60000));
        QVERIFY(spy2.takeFirst().at(0).toBool());
        QVERIFY(QFile::exists(outDocPdf));

        // -----------------------------
        // 3. Merge files
        // -----------------------------
        QStringList mergeFiles = {
                                  "C:/Users/Кирилл/Downloads/раян идеал.jpg",
                                  "D:/РезюмеБогданКС2025.pdf",
                                  "D:/Учеба/диздок.docx",
                                  "D:/Учеба/4 курс/Верификация программ/Лекция1_2019.pdf"
        };

        QString mergedPdf = QDir::currentPath() + "/facade_merged_async.pdf";

        auto* cmd3 = new MergeFilesCommand(mergeFiles, mergedPdf, &pdfEngine, &docEngine);
        QSignalSpy spy3(cmd3, SIGNAL(finished(bool)));

        queue.enqueue(cmd3);

        QVERIFY(spy3.wait(60000));
        QVERIFY(spy3.takeFirst().at(0).toBool());
        QVERIFY(QFile::exists(mergedPdf));

        // Проверяем PDF
        std::unique_ptr<Poppler::Document> mergedDoc(Poppler::Document::load(mergedPdf));
        QVERIFY(mergedDoc != nullptr);
        QVERIFY(mergedDoc->numPages() > 0);

        // -----------------------------
        // 4. Extract page
        // -----------------------------
        QString extractedImage = QDir::currentPath() + "/facade_page1_async.png";

        auto* cmd4 = new ExtractPdfPageCommand(&pdfEngine, mergedPdf, 1, extractedImage);
        QSignalSpy spy4(cmd4, SIGNAL(finished(bool)));

        queue.enqueue(cmd4);

        QVERIFY(spy4.wait(60000));
        QVERIFY(spy4.takeFirst().at(0).toBool());
        QVERIFY(QFile::exists(extractedImage));
    }
};

#endif // CONVERTERFACADE_JOB_TEST_H
