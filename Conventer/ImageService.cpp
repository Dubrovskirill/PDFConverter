#include "ImageService.h"
#include <QDebug>

ImageService::ImageService()
{

}

QImage ImageService::loadImage(const QString& path)
{
    QImage image;
    if (!image.load(path)){
        qWarning() << "Failed to load image:" << path;
    }
    return image;
}

bool ImageService::saveImage(const QImage &image, const QString &path, const char *format)
{
    if (!image.save(path, format)) {
        qWarning() << "Failed to save image:" << path;
        return false;
    }
    return true;
}

QImage ImageService::normalizeImage(const QImage &image)
{
    QImage normalized = image.convertToFormat(QImage::Format_RGB32);
    return normalized;
}
