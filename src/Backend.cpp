#include "Backend.h"
#include <QDebug>

Backend::Backend(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
}

void Backend::download(QString url)
{
    QNetworkRequest request(QUrl::fromEncoded(url.toLocal8Bit()));

    manager->get(request);
    qDebug() << url;
}

void Backend::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    qDebug() << reply->readAll();
}
