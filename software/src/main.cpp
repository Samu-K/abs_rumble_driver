#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFileInfo>
#include "Backend.h"
#include "MockSerial.h"
#include <QUdpSocket>
#include <iostream>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    auto serial = std::make_unique<MockSerial>();
    Backend *backend = new Backend(std::move(serial), &app);

    engine.rootContext()->setContextProperty("backend", backend);

    const QUrl url = QUrl::fromLocalFile(QFileInfo(u"./src/qml/main.qml"_qs).absoluteFilePath());
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);
    app.exec();

    return 0;
}
