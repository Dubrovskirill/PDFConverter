#ifndef IPDFENGINE_H
#define IPDFENGINE_H
#include <QString>
#include <QStringList>
#include <QImage>

class IPdfEngine
{
public:

    virtual ~IPdfEngine()=default;
    virtual bool openDocument(const QString& path) = 0;
    virtual int pageCount() const = 0;
    virtual QImage renderPageToImage(int pageIndex,int dpi)= 0;
    virtual bool mergeDocuments(const QStringList& paths, const QString& outputPdfPath) = 0;

};

#endif // IPDFENGINE_H
