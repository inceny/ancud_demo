#include "Backend.h"
#include <QDebug>

Backend::Backend(QQmlApplicationEngine* engine, QObject *parent) : QObject(parent)
{
    this->engine = engine;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));

    text_area_log = engine->rootObjects().at(0)->findChild<QObject*>("log");


    // создание папки для сохранения
    file_saving_location = QCoreApplication::applicationDirPath() + "/downloads";
    isDirWritable(QCoreApplication::applicationDirPath());
    QDir dir_saving_location(file_saving_location);
    if(!dir_saving_location.exists())
    {
        if(!dir_saving_location.mkpath(file_saving_location))
        {
            addLogMsg("Папка недоступна для записи", true);
        }
    }
    else
    {
        isDirWritable(file_saving_location);
    }


    // очистка папки для сохранения
    clearSaveFolder();
}

void Backend::download(QString url)
{
    clearSaveFolder();
    base_url = QUrl::fromEncoded(url.toLocal8Bit());
    QNetworkRequest request(base_url);
    manager->get(request);
}

void Backend::addLogMsg(QString msg, bool error)
{
    QString formatted_log_msg = msg;
    if(error)
    {
        formatted_log_msg.prepend("<font color= #FF0000>");
        formatted_log_msg.append("</font>");
    }

    formatted_log_msg.prepend(QString("<font color = #555555>%1 | </font>").arg(QTime::currentTime().toString("hh:mm:ss")));

    QMetaObject::invokeMethod(text_area_log, "append", Qt::DirectConnection, Q_ARG(QVariant, QVariant(formatted_log_msg)));
}

void Backend::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        addLogMsg(reply->errorString(), true);
        return;
    }

    if(reply->url() == base_url)
    {
        // если базовая страница
        QString page(reply->readAll());

        file_base_page.setFileName(file_saving_location + "/base_page.html");
        if(!file_base_page.open(QIODevice::WriteOnly))
        {
            addLogMsg("Не удалось создать файл" + file_base_page.fileName(), true);
            file_base_page.close();
            return;
        }
        file_base_page.write(page.toLocal8Bit());
        file_base_page.flush();
        file_base_page.close();

        // извлечение ссылок
        QRegularExpression re_a("(?i)<a([^>]+)>(.+?)</a>", QRegularExpression::DotMatchesEverythingOption);
        QRegularExpression re_href("\\s*(?i)href\\s*=\\s*\"(([^\"]*)\"|'[^']*'|([^'\">\\s]+))");

        QString extracted_href;
        QRegularExpressionMatchIterator i = re_a.globalMatch(page);
        while (i.hasNext())
        {
            extracted_href = re_href.match(i.next().captured()).captured(1);
            extracted_href.chop(1);

            if(!extracted_href.isEmpty() && QUrl::fromEncoded(extracted_href.toLocal8Bit()).isRelative())
            {
                extracted_href = base_url.resolved(extracted_href).toString();
                list_nested_links.append(extracted_href);

                QNetworkRequest request(extracted_href);
                manager->get(request);
            }
        }
    }
    else
    {
        // если вложенная ссылка
        QFile file_nested_page;
        file_nested_page.setFileName(file_saving_location + "/" + QString::number(list_nested_links.indexOf(reply->url().toString())) + ".html");


        if(!file_nested_page.open(QIODevice::WriteOnly))
        {
            addLogMsg("Не удалось создать файл" + file_nested_page.fileName(), true);
            file_nested_page.close();
            return;
        }
        file_nested_page.write(reply->readAll());
        file_nested_page.flush();
        file_nested_page.close();

        // добавить сообщение в лог с ссылкой на загруженный файл
        addLogMsg(QString("<a href=\"file:%2\">%1</a>").arg(reply->url().toString()).arg(file_nested_page.fileName()));
    }
}

bool Backend::isDirWritable(QString dir)
{
    // проверка папки на запись
    QFile test_file;
    test_file.setFileName(dir + "/test.txt");
    if(!test_file.open(QIODevice::WriteOnly))
    {
        addLogMsg("Папка " + dir + " недоступна для записи", true);
        return false;
    }
    else
    {
        test_file.remove();
        test_file.close();
        return true;
    }
}

void Backend::clearSaveFolder()
{
    QDir dir_saving_location(file_saving_location);
    dir_saving_location.setNameFilters(QStringList() << "*.*");
    dir_saving_location.setFilter(QDir::Files);
    foreach (QString file, dir_saving_location.entryList())
    {
        dir_saving_location.remove(file);
    }
}
