#ifndef EXTRACTPDFPAGECOMMANDTEST_H
#define EXTRACTPDFPAGECOMMANDTEST_H

#include <QtTest>
#include "../ExtractPdfPageCommand.h"
#include "../PdfEngine_Poppler.h"
#include <poppler-qt5.h>

class ExtractPdfPageCommandTest : public QObject
{
    Q_OBJECT
private slots:
    void testExtractExistingPages()
    {
        QString pdfPath =
            "D:/c++ everything/PDFConverter/build-Conventer-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/merged_specific_mixed.pdf";

        QVERIFY2(QFile::exists(pdfPath), "PDF должен существовать");

        // Создаём движок
        auto* engine = new PdfEngine_Poppler();

        // ---- Страница 1 ----
        QString out1 = QDir::currentPath() + "/page_1.png";
        ExtractPdfPageCommand cmd1(engine, pdfPath, 1, out1);
        QVERIFY2(cmd1.execute(), "Страница 1 должна быть извлечена");
        QVERIFY(QFile::exists(out1));

        // ---- Страница 20 ----
        QString out20 = QDir::currentPath() + "/page_20.png";
        ExtractPdfPageCommand cmd20(engine, pdfPath, 20, out20);

        // Загрузка PDF для проверки кол-ва страниц
        std::unique_ptr<Poppler::Document> doc(Poppler::Document::load(pdfPath));
        QVERIFY(doc != nullptr);

        bool ok20 = cmd20.execute();

        if (doc->numPages() >= 20) {
            QVERIFY(ok20);
            QVERIFY(QFile::exists(out20));
        } else {
            QVERIFY(!ok20);
            QVERIFY(!QFile::exists(out20));
        }

        delete engine;
    }

    void testExtractNonexistentPage()
    {
        QString pdfPath =
            "D:/c++ everything/PDFConverter/build-Conventer-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/merged_specific_mixed.pdf";

        QVERIFY(QFile::exists(pdfPath));

        auto* engine = new PdfEngine_Poppler();

        QString out110 = QDir::currentPath() + "/page_110.png";
        ExtractPdfPageCommand cmd(engine, pdfPath, 110, out110);

        bool ok = cmd.execute();
        QVERIFY(!ok);
        QVERIFY(!QFile::exists(out110));

        delete engine;
    }
};



#endif // EXTRACTPDFPAGECOMMANDTEST_H
