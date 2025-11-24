#include <QPdfWriter>
#include <QPainter>
#include <QDebug>

#include <qpdf/QPDF.hh>
#include <qpdf/QPDFWriter.hh>
#include <qpdf/QPDFPageDocumentHelper.hh>

#include "PdfEngine_Poppler.h"
#include "PdfWriter.h"

PdfEngine_Poppler::PdfEngine_Poppler()
{
}

PdfEngine_Poppler::~PdfEngine_Poppler()
{
    delete m_doc;
}

bool PdfEngine_Poppler::openDocument(const QString& path)
{
    delete m_doc;
    m_doc = Poppler::Document::load(path);

    if (!m_doc || m_doc->isLocked())
        return false;

    return true;
}

int PdfEngine_Poppler::pageCount() const
{
    if (!m_doc) return 0;
    return m_doc->numPages();
}

QImage PdfEngine_Poppler::renderPageToImage(int pageIndex, int dpi)
{
    if (!m_doc || pageIndex < 0 || pageIndex >= m_doc->numPages())
        return QImage();

    Poppler::Page* page = m_doc->page(pageIndex);
    if (!page)
        return QImage();


    QImage image = page->renderToImage(dpi, dpi);

    delete page;
    return image;
}

bool PdfEngine_Poppler::mergeDocuments(const QStringList& paths, const QString& outputPath)
{
    if (paths.isEmpty())
        return false;

    try {
        QPDF outPdf;
        outPdf.emptyPDF();

        QPDFPageDocumentHelper outPages(outPdf);

        for (const QString& path : paths) {
            QPDF pdf;
            pdf.processFile(path.toStdString().c_str());

            QPDFPageDocumentHelper srcPages(pdf);
            std::vector<QPDFPageObjectHelper> pages = srcPages.getAllPages();


            for (auto& pageHelper : pages) {
                QPDFObjectHandle pageObj = pageHelper.getObjectHandle();
                outPages.addPage(pageObj, false);
            }
        }

        QPDFWriter writer(outPdf, outputPath.toStdString().c_str());
        writer.write();

        return true;
    }
    catch (const std::exception& e) {
        qWarning() << "QPDF merge error:" << e.what();
        return false;
    }
}
