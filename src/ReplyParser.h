#ifndef REPLYPARSER_H
#define REPLYPARSER_H

#include <QObject>
#include <QNetworkReply>
#include <QFile>

#define FILE_EXTENSION ""

class Link;
class LinkModel;
class Backend;
class ReplyParser : public QObject
{
    Q_OBJECT
public:
    explicit ReplyParser(Backend* backend, QObject *parent = nullptr);

    QUrl base_url;
    QString file_saving_location;
    QFile file_base_page;
    QList<QNetworkReply*> list_nested_links;
    QNetworkReply* reply_base_page;

signals:
    void addLogMsg(QString msg, bool error);
    void addLink(Link* link);
    void setPercentage(QNetworkReply *reply, int percentage);

public slots:
    void onReadyRead();
    void onDownloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal);
    void replyFinished(QNetworkReply *reply);
    void download(QString url);
private:
    Backend* backend;
    QNetworkAccessManager* manager;

};

#endif // REPLYPARSER_H
