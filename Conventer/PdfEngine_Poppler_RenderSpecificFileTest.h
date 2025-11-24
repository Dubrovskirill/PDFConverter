#ifndef PDFENGINE_POPPLER_RENDERSPECIFICFILETEST_H
#define PDFENGINE_POPPLER_RENDERSPECIFICFILETEST_H
#include <QtTest>
#include "PdfEngine_Poppler.h"

class PdfEngine_Poppler_RenderSpecificFileTest : public QObject
{
    Q_OBJECT
private slots:

    void test_render_specific_pdf()
    {
        PdfEngine_Poppler engine;

        QString path = "D:/РезюмеБогданКС2025.pdf";
        QVERIFY2(engine.openDocument(path), "Не удалось открыть PDF!");

        QVERIFY2(engine.pageCount() > 0, "PDF содержит 0 страниц!");

        QImage img = engine.renderPageToImage(0);
        QVERIFY2(!img.isNull(), "Рендер дал пустое изображение!");

        QVERIFY(img.width() > 0);
        QVERIFY(img.height() > 0);

        // Сохраняем только для визуальной проверки (не ошибка теста)
        img.save("render_test_specific.png");
    }
};




#endif // PDFENGINE_POPPLER_RENDERSPECIFICFILETEST_H
