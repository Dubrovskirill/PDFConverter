#ifndef CONVERTDOCTOPDFCOMMAND_H
#define CONVERTDOCTOPDFCOMMAND_H

#include "IConverterCommand.h"
#include "IDocEngine.h"

class ConvertDocToPdfCommand : public IConverterCommand
{
    Q_OBJECT
public:
    explicit ConvertDocToPdfCommand(IDocEngine* engine,
                                    const QString& inputDoc,
                                    const QString& outputPdf,
                                    QObject* parent = nullptr);

    bool execute() override;
    void cancel() override;

private:
    IDocEngine* m_engine;
    QString m_inputDoc;
    QString m_outputPdf;
};

#endif // CONVERTDOCTOPDFCOMMAND_H
