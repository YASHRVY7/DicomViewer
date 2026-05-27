#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickImageProvider>
#include "backend/dicomcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    DicomController controller;

    engine.addImageProvider(
        "dicom",
        controller.provider()
        );
    engine.rootContext()->setContextProperty("dicomController", &controller);

    engine.loadFromModule("DicomViewer", "Main");

    return app.exec();
}
// F0C68345
