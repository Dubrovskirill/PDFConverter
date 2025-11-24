#ifndef LIBREOFFICEDOCENGINE_H
#define LIBREOFFICEDOCENGINE_H

#include "IDocEngine.h"
#include <QObject>

class LibreOfficeDocEngine : public QObject, public IDocEngine
{
    Q_OBJECT
public:
    explicit LibreOfficeDocEngine(QObject* parent = nullptr);

    bool convertToPdf(const QString& inputPath,
                      const QString& outputPdfPath) override;

private:
    QString findLibreOfficeExecutable() const;
};

#endif // LIBREOFFICEDOCENGINE_H
