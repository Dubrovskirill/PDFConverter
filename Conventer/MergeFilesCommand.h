#ifndef MERGEFILESCOMMAND_H
#define MERGEFILESCOMMAND_H

#include "IConverterCommand.h"
#include "PdfWriter.h"
#include "IPdfEngine.h"
#include "IDocEngine.h"
#include "ImageService.h"
#include <QFileInfo>
#include <QStringList>
#include <QObject>


class MergeFilesCommand : public IConverterCommand
{
    Q_OBJECT

public:

    MergeFilesCommand(const QStringList& inputFiles,
                      const QString& outputPdf,
                      IPdfEngine* pdfEngine,
                      IDocEngine* docEngine = nullptr,
                      QObject* parent = nullptr);

    bool execute() override;

private:
    QStringList m_inputFiles;
    QString m_outputPdf;
    IPdfEngine* m_pdfEngine;
    IDocEngine* m_docEngine;
};

#endif // MERGEFILESCOMMAND_H
