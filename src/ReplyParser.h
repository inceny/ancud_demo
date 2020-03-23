#ifndef REPLYPARSER_H
#define REPLYPARSER_H

#include <QObject>
#include <QNetworkReply>
#include <QFile>

class Backend;
class ReplyParser : public QObject
{
    Q_OBJECT
public:
    explicit ReplyParser(Backend* backend, QObject *parent = nullptr);

public slots:
    void onReadyRead();
    void onDownloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal);
private:
    Backend* backend;

};

#endif // REPLYPARSER_H
