#ifndef STORAGESERVICE_H
#define STORAGESERVICE_H

#include <QString>
#include <QDir>
#include <QVector>

class StorageService
{
public:
    StorageService();

    QString createTempFile(const QString& extension);

    QString tempDir() const;

    void deleteTempFiles();
private:
    QString m_tempDir;

    void ensureTempDir();
};

#endif // STORAGESERVICE_H
