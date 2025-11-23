#ifndef PDFWRITERTEST_H
#define PDFWRITERTEST_H

#include <QtTest>
#include "PdfWriter.h"
#include "ImageService.h"


class PdfWriterTest : public QObject
{
    Q_OBJECT

private slots:
    void testCreatePDF();
    void testAddMultipleImages();
    void testEndDocumentNoImages();
};
#endif // PDFWRITERTEST_H
