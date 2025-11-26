#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QStringList>
#include "ConverterFacade.h"
#include "JobQueue.h"
#include "IPdfEngine.h"
#include "IDocEngine.h"
#include "StorageService.h"

class AppController: public QObject
{
    Q_OBJECT
public:
    explicit AppController(ConverterFacade* facade,
                           JobQueue* queue,
                           IPdfEngine* pdfEngine = nullptr,
                           IDocEngine* docEngine = nullptr,
                           StorageService* storage = nullptr,
                           QObject* parent = nullptr);

public slots:
    void convertImagesAsync(QStringList files, QString outPdf);
    void convertDocAsync(QString doc, QString outPdf);
    void mergeFilesAsync(QStringList files, QString outPdf);
    void extractPageAsync(QString pdf, int page, QString outImg);
    Q_INVOKABLE void addFilesToQueue(QStringList files, bool merge, QString outPath = QString());
    Q_INVOKABLE void convertPdfToPng(QString pdfPath, int pageIndex, QString outImgPath = QString());
    Q_INVOKABLE void renderPdfPageToTempImage(QString pdfPath, int pageIndex);
    Q_INVOKABLE void cancelAllJobs();

signals:
    void imagesConverted(bool success);
    void docConverted(bool success);
    void filesMerged(bool success);
    void pageExtracted(bool success);
    void progressUpdated(int value);
    void statusUpdated(QString message);
    void previewReady(QString imagePath);

private:
    ConverterFacade* m_facade;
    JobQueue* m_queue;
    IPdfEngine* m_pdfEngine;
    IDocEngine* m_docEngine;
    StorageService* m_storage;
};

#endif // APPCONTROLLER_H
