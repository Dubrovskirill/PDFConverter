#ifndef CONVERTIMAGETOPDFCOMMAND_H
#define CONVERTIMAGETOPDFCOMMAND_H

#include "IConverterCommand.h"
#include "ImageService.h"
#include "PdfWriter.h"

#include <QString>
#include <QStringList>

class ConvertImageToPdfCommand : public IConverterCommand
{
    Q_OBJECT
public:
    ConvertImageToPdfCommand(const QStringList& images,
                             const QString& outputPdf,
                             QObject* parent = nullptr);
public slots:
    bool execute() override;

private:
    QStringList m_images;
    QString m_outputPdf;

};

#endif // CONVERTIMAGETOPDFCOMMAND_H
