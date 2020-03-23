#include "ReplyParser.h"
#include "src/Backend.h"
#include "src/linkmodel.h"

ReplyParser::ReplyParser(Backend* backend, QObject *parent) : QObject(parent)
{
    this->backend = backend;
}

void ReplyParser::onReadyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply)
    {
        return;
    }

    if(reply->url() == backend->base_url)
    {
        backend->file_base_page.write(reply->readAll());
        backend->file_base_page.flush();
    }
    else
    {
        QFile file_nested_page;
        int file_name_index = backend->list_nested_links.indexOf(reply);
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
    backend->model->setPercentage(reply, value);
}
