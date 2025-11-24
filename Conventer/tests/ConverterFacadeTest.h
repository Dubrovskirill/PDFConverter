#ifndef CONVERTERFACADE_TEST_H
#define CONVERTERFACADE_TEST_H

#include <QtTest>
#include "ConverterFacade.h"
#include "PdfEngine_Poppler.h"
#include "LibreOfficeDocEngine.h"
#include <QFile>
#include <QDir>
#include <poppler-qt5.h>

class ConverterFacadeTest : public QObject
{
    Q_OBJECT

private slots:
    void testFacadeAllOperations()
    {
        PdfEngine_Poppler pdfEngine;
        LibreOfficeDocEngine docEngine;
        ConverterFacade facade(&pdfEngine, &docEngine);

        // 1. Преобразование изображений в PDF
        QStringList images = {
                              "C:/Users/Кирилл/Downloads/3 этаж печать.jpg",
                              "C:/Users/Кирилл/Downloads/Лого 1.png",
                              "C:/Users/Кирилл/Downloads/олега режет.png",
                              "C:/Users/Кирилл/OneDrive/Pictures/Roblox/RobloxScreenShot20251019_103049833.png",
                              "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/ZMCptmrnlA.png"
        };
        QString outImagesPdf = QDir::currentPath() + "/facade_images.pdf";
        QVERIFY(facade.convertFiles(images, outImagesPdf));
        QVERIFY(QFile::exists(outImagesPdf));

        // 2. Преобразование DOC в PDF
        QString docFile = "D:/Учеба/курсовая/Идент (1).docx";
        QString outDocPdf = QDir::currentPath() + "/facade_doc.pdf";
        QVERIFY(facade.convertDoc(docFile, outDocPdf));
        QVERIFY(QFile::exists(outDocPdf));

        // 3. Объединение PDF + DOC + PDF + PNG
        QStringList mergeFiles = {
                                  outImagesPdf,
                                  "D:/РезюмеБогданКС2025.pdf",
                                  outDocPdf,
                                  "D:/Учеба/4 курс/Верификация программ/Лекция1_2019.pdf"
        };
        QString mergedPdf = QDir::currentPath() + "/facade_merged.pdf";
        QVERIFY(facade.mergeFiles(mergeFiles, mergedPdf));
        QVERIFY(QFile::exists(mergedPdf));

        // Проверяем количество страниц объединённого PDF
        std::unique_ptr<Poppler::Document> mergedDoc(Poppler::Document::load(mergedPdf));
        QVERIFY(mergedDoc != nullptr);
        QVERIFY(mergedDoc->numPages() > 0);

        // 4. Извлечение страницы
        QString extractedImage = QDir::currentPath() + "/facade_page1.png";
        QVERIFY(facade.extractPage(mergedPdf, 1, extractedImage)); // первая страница
        QVERIFY(QFile::exists(extractedImage));
    }
};

#endif // CONVERTERFACADE_TEST_H
