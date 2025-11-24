#include "ConvertImageToPdfCommandTest.h"
#include "../ConvertImageToPdfCommand.h"
#include <poppler-qt5.h>

void ConvertImageToPdfCommandTest::testImageListToPdf()
{
    QStringList images = {
                          "C:/Users/Кирилл/Downloads/3 этаж печать.jpg",
                          "C:/Users/Кирилл/Downloads/Лого 1.png",
                          "C:/Users/Кирилл/Downloads/олега режет.png",
                          "C:/Users/Кирилл/OneDrive/Pictures/Roblox/RobloxScreenShot20251019_103049833.png",
                          "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/ZMCptmrnlA.png"
    };

    QString outPdf = QDir::currentPath() + "/test_output_images_to_pdf.pdf";

    ConvertImageToPdfCommand cmd(images, outPdf);

    bool ok = cmd.execute();
    QVERIFY2(ok, "Команда должна успешно завершиться");

    QVERIFY(QFile::exists(outPdf));

    std::unique_ptr<Poppler::Document> doc(
        Poppler::Document::load(outPdf)
        );
    QVERIFY(doc != nullptr);

    QCOMPARE(doc->numPages(), images.size());
}



