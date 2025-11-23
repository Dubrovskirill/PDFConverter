#ifndef IMAGESERVICE_H
#define IMAGESERVICE_H

#include <QString>
#include <QImage>
class ImageService
{
public:
    ImageService();

    QImage loadImage(const QString& path);
    bool saveImage(const QImage& image, const QString& path, const char* format);
    QImage normalizeImage(const QImage& image);
};

#endif // IMAGESERVICE_H
