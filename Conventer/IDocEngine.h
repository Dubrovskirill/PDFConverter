#ifndef IDOCENGINE_H
#define IDOCENGINE_H
#include <QString>
class IDocEngine
{
public:
    virtual ~IDocEngine() = default;

    virtual bool convertToPdf(const QString& inputPath, const QString& outputPdfPath) = 0;
};

#endif // IDOCENGINE_H
