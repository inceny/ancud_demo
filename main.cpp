#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <src/Backend.h>
#include <src/linkmodel.h>

int main(int argc, char *argv[])
{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    LinkModel model;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("myModel", &model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    Backend backend(&engine, &model);
    engine.rootContext()->setContextProperty("backend", &backend);
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
