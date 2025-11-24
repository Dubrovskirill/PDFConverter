#include "StorageServiceTest.h"

// --- testTempDirExists ---
void StorageServiceTest::testTempDirExists()
{
    StorageService storage;                          // создаём сервис
    QString dirPath = storage.tempDir();             // получаем путь к temp-директории

    QVERIFY(!dirPath.isEmpty());                     // проверяем, что путь не пуст
    QDir dir(dirPath);                               // создаём объект QDir
    QVERIFY(dir.exists());                           // temp-директория должна существовать
}

// --- testCreateTempFile ---
void StorageServiceTest::testCreateTempFile()
{
    StorageService storage;                          // создаём сервис
    QString filePath = storage.createTempFile("png");// создаём временный файл PNG

    QVERIFY(!filePath.isEmpty());                    // путь должен быть непустым
    QFile file(filePath);                            // создаём объект файла
    QVERIFY(file.exists());                          // файл должен существовать
}

// --- testCreateTempFileExtension ---
void StorageServiceTest::testCreateTempFileExtension()
{
    StorageService storage;                          // создаём сервис
    QString filePath = storage.createTempFile("jpg");// создаём временный JPEG файл

    QVERIFY(filePath.endsWith(".jpg"));              // проверяем, что расширение корректное
}

// --- testCreateTempFileUniqueness ---
void StorageServiceTest::testCreateTempFileUniqueness()
{
    StorageService storage;                          // создаём сервис
    QString file1 = storage.createTempFile("tmp");   // создаём первый файл
    QString file2 = storage.createTempFile("tmp");   // создаём второй файл

    QVERIFY(file1 != file2);                         // имена должны быть уникальными
}

// --- testDeleteTempFiles ---
void StorageServiceTest::testDeleteTempFiles()
{
    StorageService storage;                          // создаём сервис
    QString f1 = storage.createTempFile("txt");      // создаём файл 1
    QString f2 = storage.createTempFile("txt");      // создаём файл 2

    storage.deleteTempFiles();                       // вызываем очистку temp-директории

    QFile file1(f1);                                 // создаём объект файла 1
    QFile file2(f2);                                 // создаём объект файла 2

    QVERIFY(!file1.exists());                        // файл 1 должен быть удалён
    QVERIFY(!file2.exists());                        // файл 2 должен быть удалён
}

