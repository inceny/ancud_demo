#include "ReplyParser.h"
#include "src/Backend.h"
#include "src/linkmodel.h"

ReplyParser::ReplyParser(Backend* backend, QObject *parent) : QObject(parent)
{
    this->backend = backend;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
}

void ReplyParser::download(QString url)
{
    list_nested_links.clear();
    base_url = QUrl::fromEncoded(url.toLocal8Bit());
    if(base_url.scheme() != "http" && base_url.scheme() != "https")
    {
        emit addLogMsg("Только http(s) протокол", true);
        return;
    }
    QNetworkRequest request(base_url);

    file_base_page.setFileName(backend->file_saving_location + "/base_page.html");
    if(!file_base_page.open(QIODevice::Append))
    {
        emit addLogMsg("Не удалось создать файл" + file_base_page.fileName(), true);
        file_base_page.close();
        return;
    }

    reply_base_page = manager->get(request);
    emit addLink(new Link(QString("<a href=\"file:%2\">%1</a>").arg(url).arg(file_base_page.fileName()), reply_base_page));

    connect(reply_base_page, SIGNAL(downloadProgress(qint64,qint64)), SLOT(onDownloadProgressChanged(qint64,qint64)));
    connect(reply_base_page, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

void ReplyParser::onReadyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply)
    {
        return;
    }

    if(reply->url() == base_url)
    {
        file_base_page.write(reply->readAll());
        file_base_page.flush();
    }
    else
    {
        QFile file_nested_page;
        int file_name_index = list_nested_links.indexOf(reply);
        if(file_name_index == -1)
        {
            return;
        }
        file_nested_page.setFileName(backend->file_saving_location + "/" + QString::number(file_name_index) + FILE_EXTENSION);
        if(!file_nested_page.isOpen())
        {
            file_nested_page.open(QIODevice::Append);
        }
        file_nested_page.write(reply->readAll());
        file_nested_page.flush();
    }
}

void ReplyParser::onDownloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply)
    {
        return;
    }

    int value = (int)((float)bytesReceived*100/bytesTotal);
    if(value >100 || value <0 )
    {
        return;
    }
    emit setPercentage(reply, value);
}

void ReplyParser::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        emit addLogMsg(reply->errorString(), true);
        return;
    }

    if(reply->url() == base_url)
    {
        // если базовая страница
        file_base_page.close();
        file_base_page.open(QIODevice::ReadOnly);
        QString page(file_base_page.readAll());
        file_base_page.close();
        // добавить сообщение в лог с ссылкой на загруженный файл
        emit addLogMsg(QString("Загружен файл: <a href=\"file:%2\">%1</a>").arg(reply->url().toString()).arg(file_base_page.fileName()), false);

        // извлечение ссылок
        QRegularExpression re_a("(?i)<a([^>]+)>(.+?)</a>", QRegularExpression::DotMatchesEverythingOption);
        QRegularExpression re_href("\\s*(?i)href\\s*=\\s*\"(([^\"]*)\"|'[^']*'|([^'\">\\s]+))", QRegularExpression::DotMatchesEverythingOption);

        QString extracted_href;
        QRegularExpressionMatchIterator i = re_a.globalMatch(page);
        finshed_replies = 0;
        while (i.hasNext())
        {
            extracted_href = re_href.match(i.next().captured()).captured(1);
            extracted_href.chop(1);

            if(!extracted_href.isEmpty())
            {
                if(QUrl::fromEncoded(extracted_href.toLocal8Bit()).isRelative())
                {
                    extracted_href = base_url.resolved(extracted_href).toString();
                }
//                list_nested_links.append(extracted_href);

                // создание файла сохранения вложенной страницы
                QFile file_nested_page;
                file_nested_page.setFileName(backend->file_saving_location + "/" + QString::number(list_nested_links.size()) + FILE_EXTENSION);
                if(!file_nested_page.open(QIODevice::Append))
                {
                    emit addLogMsg("Не удалось создать файл" + file_nested_page.fileName(), true);
                    file_nested_page.close();
                    return;
                }

                // создание запроса на загрузку вложенныой страницы
                QNetworkRequest request(extracted_href);
                QNetworkReply* reply_nested_link = manager->get(request);
                list_nested_links.append(reply_nested_link);

                emit addLink(new Link(QString("<a href=\"file:%2\">%1</a>").arg(reply_nested_link->url().toString()).arg(file_nested_page.fileName()), reply_nested_link));
                connect(reply_nested_link, SIGNAL(downloadProgress(qint64,qint64)), SLOT(onDownloadProgressChanged(qint64,qint64)));
                connect(reply_nested_link, SIGNAL(readyRead()), SLOT(onReadyRead()));
            }
        }
        emit addLogMsg(QString("Найдено ссылок: %1").arg(list_nested_links.size()), false);

    }
    else
    {
        // если вложенная ссылка
        QFile file_nested_page;
        int file_name_index = list_nested_links.indexOf(reply);
        if(file_name_index == -1)
        {
            return;
        }
        file_nested_page.setFileName(backend->file_saving_location + "/" + QString::number(file_name_index) + FILE_EXTENSION);
        file_nested_page.close();

        emit setPercentage(reply, 100);
        emit finishedRepliesCountChanged(++finshed_replies);

        // добавить сообщение в лог с ссылкой на загруженный файл
        emit addLogMsg(QString("Загружен файл: <a href=\"file:%2\">%1</a>").arg(reply->url().toString()).arg(file_nested_page.fileName()), false);
    }
    reply->deleteLater();
}

