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
#include <QThread>

#include "src/ReplyParser.h"

class LinkModel;
class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QQmlApplicationEngine* engine, LinkModel* model, QObject *parent = nullptr);
    ReplyParser* reply_parser;
    LinkModel* model;
    QQmlApplicationEngine* engine;

    QString file_saving_location;
    QObject* text_area_log;
    QThread* thread_reply_parser;

signals:
    void errorMsgBox(QString error_str);
    void startDownload(QString url);

public slots:
    void download(QString url);
    bool isDirWritable(QString dir);
    void addLogMsg(QString msg, bool error = false);
    void clearSaveFolder();
    void onAddLink(Link* link);
    void onSetPercentage(QNetworkReply *reply, int percentage);

private:
};

#endif // BACKEND_H
