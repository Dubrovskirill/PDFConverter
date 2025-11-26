#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "AppController.h"
#include "ConverterFacade.h"
#include "PdfEngine_Poppler.h"
#include "LibreOfficeDocEngine.h"
#include "JobQueue.h"
#include "StorageService.h"

#include <iostream>


#include <QtTest>
#include <QCoreApplication>
#include <QImage>
#include <QGuiApplication>
#include <QDebug>



int main(int argc, char *argv[])
{


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    std::cout << "Backend is working..." << std::endl;


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    // --- backend objects (DI) ---
    auto* pdfEngine = new PdfEngine_Poppler();
    auto* docEngine = new LibreOfficeDocEngine();
    auto* queue = new JobQueue(&app);
    auto* storage = new StorageService();
    auto* facade = new ConverterFacade(pdfEngine, docEngine, &app);
    auto* controller = new AppController(facade, queue, pdfEngine, docEngine, storage, &app);

    engine.rootContext()->setContextProperty("appController", controller);

    engine.load(url);

    return app.exec();



}
