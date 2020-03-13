#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);

signals:

public slots:
    void download(QString url);
    void replyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* manager;
};

#endif // BACKEND_H
