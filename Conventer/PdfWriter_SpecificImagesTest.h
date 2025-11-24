#ifndef PDFWRITER_SPECIFICIMAGESTEST_H
#define PDFWRITER_SPECIFICIMAGESTEST_H
#include <QtTest>
#include "PdfWriter.h"

class PdfWriter_SpecificImagesTest : public QObject
{
    Q_OBJECT
private slots:

    void test_convert_specific_images()
    {
        PdfWriter writer;

        QString output = "SpecificImagesTest.pdf";
        QVERIFY2(writer.beginDocument(output), "Не удалось создать PDF!");

        QStringList images = {
                              "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/TaskTracker_0Xh0dgn6WJ.png",
                              "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/vmware_1hK6eyfUC4.png",
                              "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/PrintGraphs_rw7WgGMu3M.png",
                              "C:/Users/Кирилл/Downloads/3 этаж печать.jpg"
        };

        for (const QString& path : images)
        {
            QImage img(path);
            QVERIFY2(!img.isNull(), qPrintable(QString("Не удалось открыть изображение: ") + path));
            QVERIFY2(writer.addImage(img), qPrintable(QString("Не удалось добавить изображение: ") + path));
        }

        QVERIFY2(writer.endDocument(), "PDF не был корректно закрыт!");

        QFile f(output);
        QVERIFY2(f.exists(), "Итоговый PDF не найден на диске!");
        QVERIFY(f.size() > 1000);  // минимальная проверка размера
    }
};



#endif // PDFWRITER_SPECIFICIMAGESTEST_H
