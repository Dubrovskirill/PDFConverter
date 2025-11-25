#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QStringList>
#include "ConverterFacade.h"
#include "JobQueue.h"
#include "IPdfEngine.h"
#include "IDocEngine.h"

class AppController: public QObject
{
    Q_OBJECT
public:
    explicit AppController(ConverterFacade* facade,
                           JobQueue* queue,
                           IPdfEngine* pdfEngine = nullptr,
                           IDocEngine* docEngine = nullptr,
                           QObject* parent = nullptr);

public slots:
    void convertImagesAsync(QStringList files, QString outPdf);
    void convertDocAsync(QString doc, QString outPdf);
    void mergeFilesAsync(QStringList files, QString outPdf);
    void extractPageAsync(QString pdf, int page, QString outImg);

signals:
    void imagesConverted(bool success);
    void docConverted(bool success);
    void filesMerged(bool success);
    void pageExtracted(bool success);
    void progressUpdated(int value);
    void statusUpdated(QString message);

private:
    ConverterFacade* m_facade;
    JobQueue* m_queue;
    IPdfEngine* m_pdfEngine;
    IDocEngine* m_docEngine;
};

#endif // APPCONTROLLER_H
