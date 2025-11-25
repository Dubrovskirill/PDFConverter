#ifndef STORAGESERVICE_H
#define STORAGESERVICE_H

#include <QString>
#include <QDir>
#include <QVector>
#include <QMutex>

class StorageService
{
public:
    StorageService();

    QString createTempFile(const QString& extension);
    QString createTempDirForJob();

    QString tempDir() const;

    void deleteTempFiles();
    void deleteTempDir(const QString& dirPath);
private:
    QString m_tempDir;
    QVector<QString> m_tempFiles;
    QVector<QString> m_tempDirs;
    mutable QMutex m_mutex;

    void ensureTempDir();
};

#endif // STORAGESERVICE_H
