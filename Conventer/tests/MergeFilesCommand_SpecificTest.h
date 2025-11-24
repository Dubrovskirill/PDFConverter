#ifndef MERGEFILESCOMMAND_SPECIFICTEST_H
#define MERGEFILESCOMMAND_SPECIFICTEST_H

#include <QtTest>
#include "../MergeFilesCommand.h"
#include "../PdfEngine_Poppler.h"
#include <poppler-qt5.h>
#include <QFile>
#include <QDir>

class MergeFilesCommand_SpecificTest : public QObject
{
    Q_OBJECT

private slots:
    void testMergeMixedFiles()
    {
        // Список смешанных файлов (PNG + PDF + PNG)
        QStringList files = {
                             "C:/Users/Кирилл/Downloads/3 этаж печать.jpg",
                             "D:/РезюмеБогданКС2025.pdf",
                             "C:/Users/Кирилл/Downloads/Лого 1.png",
                             "C:/Users/Кирилл/Downloads/олега режет.png",
                             "C:/Users/Кирилл/OneDrive/Pictures/Roblox/RobloxScreenShot20251019_103049833.png",
                             "D:/Учеба/4 курс/Верификация программ/Лекция1_2019.pdf",
                             "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/ZMCptmrnlA.png"
    };

        // Итоговый PDF в папке билда
        QString outPdf = QDir::currentPath() + "/merged_specific_mixed.pdf";

        // Создаём PdfEngine_Poppler для mergeDocuments
        PdfEngine_Poppler engine;

        MergeFilesCommand cmd(files, outPdf, &engine);

        // Выполняем команду
        bool ok = cmd.execute();
        QVERIFY2(ok, "Команда MergeFilesCommand должна успешно завершиться");

        // Проверяем, что PDF создан
        QVERIFY(QFile::exists(outPdf));

        // Загружаем PDF и проверяем количество страниц
        std::unique_ptr<Poppler::Document> doc(Poppler::Document::load(outPdf));
        QVERIFY(doc != nullptr);

        // Минимальная проверка: PDF должен иметь хотя бы столько страниц, сколько исходных PDF + PNG
        // PNG превращаются в 1 страницу каждая, PDF добавляются целиком
        int expectedMinPages = 0;
        for (const QString& f : files) {
            QString suf = QFileInfo(f).suffix().toLower();
            if (suf == "pdf") {
                std::unique_ptr<Poppler::Document> d(Poppler::Document::load(f));
                if (d) expectedMinPages += d->numPages();
            } else if (suf == "png" || suf == "jpg" || suf == "jpeg") {
                expectedMinPages += 1;
            }
        }

        QCOMPARE(doc->numPages(), expectedMinPages);
    }
};



#endif // MERGEFILESCOMMAND_SPECIFICTEST_H
