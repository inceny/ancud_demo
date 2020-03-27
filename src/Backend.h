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
    Q_PROPERTY(int finishedRepliesCount READ finishedRepliesCount WRITE setFinishedRepliesCount NOTIFY finishedRepliesCountChanged)
public:
    explicit Backend(QObject *parent = nullptr);
    ReplyParser* reply_parser;
    LinkModel* model;
    QQmlApplicationEngine* engine;

    QString file_saving_location;
    QObject* text_area_log;
    QThread* thread_reply_parser;

    int finshed_replies;
    int finishedRepliesCount();

    int init(QQmlApplicationEngine *engine, LinkModel *model);
signals:
    void errorMsgBox(QString error_str);
    void startDownload(QString url);
    void finishedRepliesCountChanged();

public slots:
    void download(QString url);
    bool isDirWritable(QString dir);
    void addLogMsg(QString msg, bool error = false);
    void clearSaveFolder();
    void onAddLink(Link* link);
    void onSetPercentage(QNetworkReply *reply, int percentage);
    void setFinishedRepliesCount(int count);
private:
};

#endif // BACKEND_H
