#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QQmlApplicationEngine>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QQmlApplicationEngine* engine, QObject *parent = nullptr);

signals:

public slots:
    void download(QString url);
    void replyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* manager;
    QQmlApplicationEngine* engine;
    QObject* text_area_log;
    QUrl base_url;
};

#endif // BACKEND_H
