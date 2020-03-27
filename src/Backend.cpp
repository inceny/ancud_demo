#include "Backend.h"
#include <QDebug>
#include <src/linkmodel.h>

Backend::Backend(QObject *parent) : QObject(parent)
{
    reply_parser = 0;
    finshed_replies = 0;
}

int Backend::init(QQmlApplicationEngine* engine, LinkModel* model)
{
    this->engine = engine;
    this->model = model;
    emit finishedRepliesCountChanged();
    text_area_log = engine->rootObjects().at(0)->findChild<QObject*>("log");

    reply_parser = new ReplyParser(this);
    connect(this, SIGNAL(startDownload(QString)), reply_parser, SLOT(download(QString)));
    connect(reply_parser, SIGNAL(addLogMsg(QString, bool)), this, SLOT(addLogMsg(QString, bool)));
    connect(reply_parser, SIGNAL(addLink(Link*)), this, SLOT(onAddLink(Link*)));
    connect(reply_parser, SIGNAL(setPercentage(QNetworkReply*, int)), this, SLOT(onSetPercentage(QNetworkReply*, int)));
    connect(reply_parser, SIGNAL(finishedRepliesCountChanged(int)), this, SLOT(setFinishedRepliesCount(int)));
    thread_reply_parser = new QThread();
    reply_parser->moveToThread(thread_reply_parser);
    thread_reply_parser->start();


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

int Backend::finishedRepliesCount()
{
    return finshed_replies;
}

void Backend::download(QString url)
{
    clearSaveFolder();
    emit startDownload(url);
}

void Backend::onAddLink(Link *link)
{
    model->addLink(link);
}

void Backend::onSetPercentage(QNetworkReply *reply, int percentage)
{
    model->setPercentage(reply, percentage);
}

void Backend::setFinishedRepliesCount(int count)
{
    finshed_replies = count;
    emit finishedRepliesCountChanged();
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
//    dir_saving_location.setNameFilters(QStringList() << "*.*");
    dir_saving_location.setFilter(QDir::Files);
    foreach (QString file, dir_saving_location.entryList())
    {
        dir_saving_location.remove(file);
    }

    model->clearList();
}
