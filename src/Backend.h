#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QQmlApplicationEngine* engine, QObject *parent = nullptr);

signals:
    void errorMsgBox(QString error_str);
public slots:
    void download(QString url);
    void replyFinished(QNetworkReply* reply);
    bool isDirWritable(QString dir);
    void addLogMsg(QString msg, bool error = false);

private:
    QNetworkAccessManager* manager;
    QQmlApplicationEngine* engine;
    QObject* text_area_log;
    QObject* msg_box;
    QUrl base_url;
    QString file_saving_location;
    QFile file_base_page;
};

#endif // BACKEND_H
