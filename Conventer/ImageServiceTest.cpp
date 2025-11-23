#include "ImageServiceTest.h"
#include <QImage>
#include <QFile>
#include <QDir>

void ImageServiceTest::testLoadImage()
{
    ImageService service;                            // создаём объект сервиса
    QImage image(100, 100, QImage::Format_RGB32);   // создаём тестовое изображение
    QString path = QDir::tempPath() + "/test.png";  // временный путь
    image.save(path, "PNG");                         // сохраняем тестовое изображение

    QImage loaded = service.loadImage(path);        // загружаем изображение через сервис
    QVERIFY(!loaded.isNull());                      // проверяем, что изображение загружено

    QFile::remove(path);                             // удаляем временный файл
}

void ImageServiceTest::testLoadImageFail()
{
    ImageService service;
    QImage loaded = service.loadImage("non_existing_file.png"); // загружаем несуществующий файл
    QVERIFY(loaded.isNull());                    // изображение должно быть пустым
}

void ImageServiceTest::testSaveImage()
{
    ImageService service;
    QImage image(50, 50, QImage::Format_RGB32);           // создаём тестовое изображение
    QString path = QDir::tempPath() + "/save_test.png";   // временный путь

    bool result = service.saveImage(image, path, "PNG");  // сохраняем через сервис
    QVERIFY(result);                                      // проверяем, что функция вернула true
    QVERIFY(QFile::exists(path));                         // проверяем, что файл реально создан

    QFile::remove(path);                                  // удаляем временный файл
}

void ImageServiceTest::testNormalizeImage()
{
    ImageService service;
    QImage image(10, 10, QImage::Format_ARGB32);   // изображение в формате ARGB32
    QImage normalized = service.normalizeImage(image); // нормализуем через сервис

    QCOMPARE(normalized.format(), QImage::Format_RGB32); // проверяем, что формат изменился на RGB32
}
