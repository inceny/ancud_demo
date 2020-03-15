#include "Backend.h"
#include <QDebug>

Backend::Backend(QQmlApplicationEngine* engine, QObject *parent) : QObject(parent)
{
    this->engine = engine;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));

    text_area_log = engine->rootObjects().at(0)->findChild<QObject*>("log");
}

void Backend::download(QString url)
{
    base_url = QUrl::fromEncoded(url.toLocal8Bit());
    QNetworkRequest request(base_url);
    manager->get(request);
}

void Backend::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        QMetaObject::invokeMethod(text_area_log, "append", Qt::DirectConnection,
                                  Q_ARG(QVariant, QVariant("Error:" + reply->errorString())));
        return;
    }



    if(reply->url() == base_url)
    {
        // извлечение ссылок
        QString page(reply->readAll());
        QRegularExpression re_a("(?i)<a([^>]+)>(.+?)</a>");
        QRegularExpression re_href("\\s*(?i)href\\s*=\\s*(\"([^\"]*\")|'[^']*'|([^'\">\\s]+))");

        QString extracted_href;
        QRegularExpressionMatchIterator i = re_a.globalMatch(page);
        while (i.hasNext())
        {
            QRegularExpressionMatch match_a_tag = i.next();
            extracted_href = re_href.match(match_a_tag.captured()).captured();

            // если сслыка в ""
            if(extracted_href.endsWith("\""))
            {
                extracted_href.remove(0, extracted_href.indexOf("\"") + 1);
            }
            // если ссылка в ''
            else if(extracted_href.endsWith("\'"))
            {
                extracted_href.remove(0, extracted_href.indexOf("\'") + 1);
            }
            extracted_href.chop(1);

            if(!extracted_href.isEmpty())
            {
                QNetworkRequest request(extracted_href);
                manager->get(request);

                QMetaObject::invokeMethod(text_area_log, "append", Qt::DirectConnection,
                                          Q_ARG(QVariant, QVariant(extracted_href)));
            }
        }
    }
    else
    {
        qDebug() << reply->bytesAvailable();
        reply->readAll();
    }

}
