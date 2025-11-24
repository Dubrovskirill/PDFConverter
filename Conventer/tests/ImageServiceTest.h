#ifndef IMAGESERVICETEST_H
#define IMAGESERVICETEST_H

#include <QtTest>
#include "ImageService.h"


class ImageServiceTest : public QObject
{
    Q_OBJECT

private slots:
    void testLoadImage();
    void testLoadImageFail();
    void testSaveImage();
    void testNormalizeImage();
};

#endif // IMAGESERVICETEST_H
