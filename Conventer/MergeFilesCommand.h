#ifndef MERGEFILESCOMMAND_H
#define MERGEFILESCOMMAND_H
#include "IConverterCommand.h"
#include "PdfWriter.h"
#include "IPdfEngine.h"
#include "ImageService.h"
#include <QFileInfo>


class MergeFilesCommand: public IConverterCommand
{
    Q_OBJECT
public:
    MergeFilesCommand(const QStringList& inputFiles,
                      const QString& outputPdf,
                      IPdfEngine* pdfEngine,
                      QObject* parent = nullptr);
    bool execute() override;
private:
    QStringList m_inputFiles;
    QString m_outputPdf;
    IPdfEngine* m_pdfEngine;
};

#endif // MERGEFILESCOMMAND_H
