#ifndef PDFENGINE_POPPLER_MERGESPECIFICTEST_H
#define PDFENGINE_POPPLER_MERGESPECIFICTEST_H

#include <QtTest>
#include "PdfEngine_Poppler.h"
#include <QFile>
#include <QCoreApplication>
#include <QDir>

class PdfEngine_Poppler_MergeSpecificTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        qDebug() << "Starting mergeDocuments specific files test";
    }

    void testMergeSpecificPDFs() {
        PdfEngine_Poppler engine;

        QStringList inputFiles = {
                                  "C:/Users/Кирилл/Downloads/Лекция_15___копия___копия.pdf",
                                  "D:/РезюмеБогданКС2025.pdf",
                                  "C:/Users/Кирилл/Downloads/Certificate_fc643cbe-9a5c-4707-bc16-ed73ca8fafc6.pdf"
        };

        // Папка билда (рядом с exe)
        QString buildDir = QCoreApplication::applicationDirPath();
        QString outputPath = buildDir + "/merged_specific.pdf";

        // Проверяем что mergeDocuments возвращает true
        bool ok = engine.mergeDocuments(inputFiles, outputPath);
        QVERIFY(ok);

        // Проверяем, что файл создан
        QVERIFY(QFile::exists(outputPath));

        // Проверяем, что PDF можно открыть и есть хотя бы 1 страница
        QVERIFY(engine.openDocument(outputPath));
        QVERIFY(engine.pageCount() > 0);

        qDebug() << "Merged PDF saved to:" << outputPath << "with pages:" << engine.pageCount();
    }

    void cleanupTestCase() {
        qDebug() << "Finished mergeDocuments specific files test";
    }
};

#endif // PDFENGINE_POPPLER_MERGESPECIFICTEST_H
