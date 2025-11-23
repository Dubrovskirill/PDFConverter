#include "StorageService.h"
#include <QStandardPaths>
#include <QFile>
#include <QDebug>
#include <QDateTime>

StorageService::StorageService()
{
    QString baseTemp = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    m_tempDir = baseTemp + "/MyPdfAppTemp";
    ensureTempDir();
}

void StorageService::ensureTempDir()
{
    QDir dir(m_tempDir);
    if(!dir.exists()) {
        dir.mkpath(".");
    }
}

QString StorageService::tempDir() const
{
    return m_tempDir;
}

QString StorageService::createTempFile(const QString& extension)
{
    ensureTempDir();

    QString filename = QString("%1/temp_%2.%3")
                           .arg(m_tempDir)
                           .arg(QString::number(QDateTime::currentMSecsSinceEpoch()))
                           .arg(extension);

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly)) {
        qWarning()<<"Cannot create temp file: "<<filename;
        return {};
    }
    file.close();
    return filename;
}

void StorageService::deleteTempFiles()
{
    QDir dir(m_tempDir);

    if(!dir.exists())
        return;

    auto files = dir.entryList(QDir::Files);
    for (const auto& f: files) {
        dir.remove(f);
    }
}
