#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickImageProvider>
#include <QtQml>
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

    qmlRegisterSingletonInstance(
        "DicomViewer.Backend",
        1,
        0,
        "DicomController",
        &controller
        );

    engine.loadFromModule("DicomViewer", "Main");

    return app.exec();
}
// F0C68345
