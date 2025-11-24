#include "PdfWriterTest.h"
#include <QFile>
#include <QDir>

void PdfWriterTest::testCreatePDF()
{
    PdfWriter writer;                                  // создаём объект PdfWriter
    QString path = QDir::tempPath() + "/test.pdf";     // временный путь для PDF

    writer.beginDocument(path);                         // начинаем документ

    ImageService imgService;                            // создаём тестовое изображение
    QImage image(100, 100, QImage::Format_RGB32);      // создаём простое изображение
    writer.addImage(image);                             // добавляем изображение

    QVERIFY(writer.endDocument());                      // завершаем PDF, проверяем успех
    QVERIFY(QFile::exists(path));                       // проверяем, что файл создан

    QFile::remove(path);                                // удаляем временный файл
}

void PdfWriterTest::testAddMultipleImages()
{
    PdfWriter writer;
    QString path = QDir::tempPath() + "/multi_test.pdf";

    writer.beginDocument(path);

    ImageService imgService;
    QImage img1(50, 50, QImage::Format_RGB32);
    QImage img2(60, 60, QImage::Format_RGB32);

    writer.addImage(img1);  // добавляем первое изображение
    writer.addImage(img2);  // добавляем второе изображение

    QVERIFY(writer.endDocument());          // завершаем PDF
    QVERIFY(QFile::exists(path));           // проверяем, что PDF существует

    QFile::remove(path);                    // удаляем файл
}

void PdfWriterTest::testEndDocumentNoImages()
{
    PdfWriter writer;
    QString path = QDir::tempPath() + "/empty_test.pdf";

    writer.beginDocument(path);             // начинаем PDF, но не добавляем изображений

    QVERIFY(!writer.endDocument());         // endDocument должен вернуть false
    QVERIFY(!QFile::exists(path));          // файл не должен быть создан
}
