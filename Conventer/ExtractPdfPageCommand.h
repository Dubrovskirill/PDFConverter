#ifndef EXTRACTPDFPAGECOMMAND_H
#define EXTRACTPDFPAGECOMMAND_H

#include <QObject>
#include "IConverterCommand.h"
#include "IPdfEngine.h"
#include "ImageService.h"

class ExtractPdfPageCommand: public IConverterCommand
{
    Q_OBJECT
public:
    explicit ExtractPdfPageCommand(IPdfEngine* engine,
                                   const QString& pdfPath,
                                   int pageIndex,
                                   const QString& outputImage,
                                   QObject* parent = nullptr);
    ~ExtractPdfPageCommand() override = default;
public slots:
    bool execute() override;
    void cancel() override;

private:
    IPdfEngine* m_engine;
    QString m_pdfPath;
    int m_pageIndex;
    QString m_outputImage;
    ImageService m_imageService;

};

#endif // EXTRACTPDFPAGECOMMAND_H
