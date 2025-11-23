#ifndef PDFWRITER_H
#define PDFWRITER_H

#include <QString>
#include <QImage>
#include <QVector>


class PdfWriter
{
public:
    PdfWriter();

    bool beginDocument(const QString& path);
    bool addImage(const QImage& image);
    bool endDocument();

private:
    QString m_pdfPath;
    QVector<QImage> m_images;
};

#endif // PDFWRITER_H
