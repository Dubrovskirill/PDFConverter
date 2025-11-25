#include "StorageService.h"
#include <QStandardPaths>
#include <QDir>
#include <QTemporaryFile>
#include <QFile>
#include <QDebug>
#include <QUuid>
#include <QMutexLocker>

StorageService::StorageService() 
{
    ensureTempDir();
}

void StorageService::ensureTempDir()
{
    if (!m_tempDir.isEmpty())
        return;

    QString base = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    m_tempDir = QDir(base).filePath("PDFConverter");
    QDir dir(m_tempDir);
    if (!dir.exists())
        dir.mkpath(".");
}

QString StorageService::tempDir() const
{
    return m_tempDir;
}

QString StorageService::createTempFile(const QString& extension)
{
    ensureTempDir();

    QString ext = extension.startsWith('.') ? extension.mid(1) : extension;
    QTemporaryFile tmp(QDir(m_tempDir).filePath(QString("pdfconv_XXXXXX.%1").arg(ext)));
    tmp.setAutoRemove(false);
    if (!tmp.open()) {
        qWarning() << "Cannot create temp file:" << tmp.fileName();
        return QString();
    }

    QString path = tmp.fileName();
    tmp.close();

    {
        QMutexLocker locker(&m_mutex);
        m_tempFiles.append(path);
    }

    return path;
}

QString StorageService::createTempDirForJob()
{
    ensureTempDir();
    QString dirPath = QDir(m_tempDir).filePath(QUuid::createUuid().toString());
    QDir d;
    d.mkpath(dirPath);

    { QMutexLocker locker(&m_mutex); m_tempDirs.append(dirPath); }
    return dirPath;
}

void StorageService::deleteTempFiles()
{
    QMutexLocker locker(&m_mutex);
    for (const QString &f : m_tempFiles)
        QFile::remove(f);
    m_tempFiles.clear();

    for (const QString &dpath : m_tempDirs) {
        QDir dir(dpath);
        dir.removeRecursively();
    }
    m_tempDirs.clear();
}

void StorageService::deleteTempDir(const QString& dirPath)
{
    QMutexLocker locker(&m_mutex);
    QDir dir(dirPath);
    dir.removeRecursively();
    m_tempDirs.removeAll(dirPath);
}
