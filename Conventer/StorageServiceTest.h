#ifndef STORAGESERVICETEST_H
#define STORAGESERVICETEST_H

#include <QtTest>              // фреймворк Qt Test
#include "StorageService.h"    // тестируемый класс

class StorageServiceTest : public QObject
{
    Q_OBJECT

private slots:
    void testTempDirExists();          // проверка, что temp-директория создана
    void testCreateTempFile();         // проверка создания временного файла
    void testCreateTempFileExtension(); // проверка правильного расширения
    void testCreateTempFileUniqueness(); // проверка уникальности имён
    void testDeleteTempFiles();         // проверка очистки временных файлов
};

#endif // STORAGESERVICETEST_H
