#include "PdfEngine_Poppler.h"

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

    // Рендер в QImage
    QImage image = page->renderToImage(dpi, dpi);

    delete page;
    return image;
}

bool PdfEngine_Poppler::mergeDocuments(const QStringList&, const QString&)
{
    // Пока заглушка — реализация будет позже
    return false;
}
