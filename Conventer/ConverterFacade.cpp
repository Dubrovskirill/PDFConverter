#include "ConverterFacade.h"
#include "ConvertImageToPdfCommand.h"
#include "ConvertDocToPdfCommand.h"
#include "MergeFilesCommand.h"
#include "ExtractPdfPageCommand.h"
#include <QDebug>

ConverterFacade::ConverterFacade(IPdfEngine* pdfEngine,
                                 IDocEngine* docEngine,
                                 QObject* parent)
    : QObject(parent)
    , m_pdfEngine(pdfEngine)
    , m_docEngine(docEngine)
{
}

bool ConverterFacade::convertFiles(const QStringList& images, const QString& outputPdf)
{
    ConvertImageToPdfCommand cmd(images, outputPdf);
    return cmd.execute();
}

bool ConverterFacade::mergeFiles(const QStringList& files, const QString& outputPdf)
{
    MergeFilesCommand cmd(files, outputPdf, m_pdfEngine, m_docEngine);
    return cmd.execute();
}

bool ConverterFacade::extractPage(const QString& pdfPath, int pageIndex, const QString& outputImage)
{
    ExtractPdfPageCommand cmd(m_pdfEngine, pdfPath, pageIndex, outputImage);
    return cmd.execute();
}

bool ConverterFacade::convertDoc(const QString& docPath, const QString& outputPdf)
{
    ConvertDocToPdfCommand cmd(m_docEngine, docPath, outputPdf);
    return cmd.execute();
}
