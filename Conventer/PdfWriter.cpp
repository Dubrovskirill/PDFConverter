#include "PdfWriter.h"
#include <QPdfWriter>
#include <QPainter>
#include <QDebug>

PdfWriter::PdfWriter()
{

}

bool PdfWriter::beginDocument(const QString &path)
{
    m_pdfPath = path;
    m_images.clear();
    return true;
}

bool PdfWriter::addImage(const QImage &image)
{
    if( image.isNull()) {
        qWarning() << "Cannot add null image to PDF";
        return false;
    }
    m_images.append(image);
    return true;
}

bool PdfWriter::endDocument()
{
    if (m_pdfPath.isEmpty()) {
        qWarning() << "No PDF path specified";
        return false;
    }

    if (m_images.isEmpty()) {
        qWarning() << "No images to write to PDF";
        return false;
    }

    QPdfWriter writer (m_pdfPath);
    QPainter painter(&writer);

    QSize pageSize(writer.width(), writer.height());
    for (int i = 0; i < m_images.size(); ++i) {
        const QImage& img = m_images[i];

        QImage scaled = img.scaled(pageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);


        int x = (pageSize.width() - scaled.width()) / 2;
        int y = (pageSize.height() - scaled.height()) / 2;
        painter.drawImage(x, y, scaled);

        if (i != m_images.size() - 1) {
            writer.newPage();
        }
    }


    painter.end();
    m_images.clear();
    return true;

}

