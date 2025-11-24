#ifndef CONVERTERFACADE_H
#define CONVERTERFACADE_H

#include <QObject>
#include <QStringList>
#include "IPdfEngine.h"
#include "IDocEngine.h"

class ConverterFacade: public QObject
{
    Q_OBJECT
public:
    explicit ConverterFacade(IPdfEngine* pdfEngine,
                             IDocEngine* docEngine,
                             QObject* parent = nullptr);
    bool convertFiles(const QStringList& images, const QString& outputPdf);

    bool mergeFiles(const QStringList& files, const QString& outputPdf);

    bool extractPage(const QString& pdfPath, int pageIndex, const QString& outputImage);

    bool convertDoc(const QString& docPath, const QString& outputPdf);
private:
    IPdfEngine* m_pdfEngine;
    IDocEngine* m_docEngine;
};

#endif // CONVERTERFACADE_H
