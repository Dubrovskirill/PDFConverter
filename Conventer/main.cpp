#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <iostream>


#include <QtTest>

//#include "PdfWriterTest.h"
//QTEST_MAIN(PdfWriterTest)
#include <QCoreApplication>
#include "PdfWriter.h"
#include <QImage>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PdfWriter writer;

    // Сохраним PDF в папку "Документы" пользователя
    QString pdfPath = "C:/Users/Кирилл/Documents/MyPdfFromImages.pdf";

    if (!writer.beginDocument(pdfPath)) {
        qWarning() << "Failed to start PDF document";
        return -1;
    }

    // Пути к твоим изображениям
    QStringList images = {
                          "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/TaskTracker_0Xh0dgn6WJ.png",
                          "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/vmware_1hK6eyfUC4.png",
                          "C:/Users/Кирилл/OneDrive/Документы/ShareX/Screenshots/2025-11/PrintGraphs_rw7WgGMu3M.png"
    };

    for (const QString &path : images) {
        QImage img(path);
        if (!writer.addImage(img)) {
            qWarning() << "Failed to add image:" << path;
        }
    }

    if (!writer.endDocument()) {
        qWarning() << "Failed to save PDF";
        return -1;
    }

    qDebug() << "PDF successfully saved to:" << pdfPath;
    return 0;
}

//int main(int argc, char *argv[])
//{


//#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//#endif
//    QGuiApplication app(argc, argv);
//    std::cout << "Backend is working..." << std::endl;


////    QQmlApplicationEngine engine;
////    const QUrl url(QStringLiteral("qrc:/main.qml"));
////    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
////        &app, [url](QObject *obj, const QUrl &objUrl) {
////            if (!obj && url == objUrl)
////                QCoreApplication::exit(-1);
////        }, Qt::QueuedConnection);
////    engine.load(url);

//    return app.exec();



//}
