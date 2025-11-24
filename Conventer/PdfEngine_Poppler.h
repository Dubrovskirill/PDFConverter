#ifndef PDFENGINE_POPPLER_H
#define PDFENGINE_POPPLER_H

#include <poppler-qt5.h>

#include "IPdfEngine.h"
class PdfEngine_Poppler: public IPdfEngine
{
public:
    PdfEngine_Poppler();
    ~PdfEngine_Poppler();
    bool openDocument(const QString& path) override;
    int pageCount() const override;
    QImage renderPageToImage(int pageIndex, int dpi = 1500) override;
    bool mergeDocuments(const QStringList& paths, const QString& outputPath) override;
private:
    Poppler::Document* m_doc = nullptr;
};

#endif // PDFENGINE_POPPLER_H
