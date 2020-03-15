#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <src/Backend.h>

int main(int argc, char *argv[])
{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    Backend backend(&engine);
    engine.rootContext()->setContextProperty("backend", &backend);
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
