#ifndef STORAGESERVICETEST_H
#define STORAGESERVICETEST_H

#include <QtTest>
#include "StorageService.h"

class StorageServiceTest : public QObject
{
    Q_OBJECT

private slots:
    void testTempDirExists();
    void testCreateTempFile();
    void testCreateTempFileExtension();
    void testCreateTempFileUniqueness();
    void testDeleteTempFiles();
};

#endif // STORAGESERVICETEST_H
