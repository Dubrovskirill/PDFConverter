#ifndef LIBREOFFICEDOCENGINE_CONVERTERTEST_H
#define LIBREOFFICEDOCENGINE_CONVERTERTEST_H

#include <QtTest>
#include "LibreOfficeDocEngine.h"
#include <QFile>
#include <QCoreApplication>
#include <QDir>

class LibreOfficeDocEngine_ConverterTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        qDebug() << "Starting LibreOfficeDocEngine converter specific test";
    }

    void testConvertDocToPdf() {
        LibreOfficeDocEngine engine;

        // Входной DOCX файл для теста
        QString inputDoc = "D:/Учеба/курсовая/Идент (1).docx";

        // Папка билда (рядом с exe)
        QString buildDir = QCoreApplication::applicationDirPath();
        QString outputPdf = buildDir + "/test_doc_to_pdf.pdf";

        // Проверяем, что конвертация возвращает true
        bool ok = engine.convertToPdf(inputDoc, outputPdf);
        QVERIFY(ok);

        // Проверяем, что PDF создан
        QVERIFY(QFile::exists(outputPdf));

        qDebug() << "PDF successfully created:" << outputPdf;
    }

    void cleanupTestCase() {
        qDebug() << "Finished LibreOfficeDocEngine converter specific test";
    }
};

#endif // LIBREOFFICEDOCENGINE_CONVERTERTEST_H
