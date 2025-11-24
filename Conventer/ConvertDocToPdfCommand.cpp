#include "ConvertDocToPdfCommand.h"
#include <QDebug>
#include <atomic>

ConvertDocToPdfCommand::ConvertDocToPdfCommand(IDocEngine* engine,
                                               const QString& inputDoc,
                                               const QString& outputPdf,
                                               QObject* parent)
    : IConverterCommand(parent)
    , m_engine(engine)
    , m_inputDoc(inputDoc)
    , m_outputPdf(outputPdf)
{
}

bool ConvertDocToPdfCommand::execute()
{
    reportStatus("Начата конвертация DOC в PDF");

    if (!m_engine) {
        reportStatus("Ошибка: IDocEngine не задан");
        emit finished(false);
        return false;
    }

    if (m_cancelRequested) {
        reportStatus("Операция отменена");
        emit finished(false);
        return false;
    }

    if (!m_engine->convertToPdf(m_inputDoc, m_outputPdf)) {
        reportStatus(QString("Ошибка конвертации документа: %1").arg(m_inputDoc));
        emit finished(false);
        return false;
    }

    reportStatus("Документ успешно конвертирован в PDF");
    emit finished(true);
    return true;
}

void ConvertDocToPdfCommand::cancel()
{
    m_cancelRequested = true;
    reportStatus("Операция отменена");
}
