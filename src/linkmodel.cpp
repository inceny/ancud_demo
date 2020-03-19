#include "linkmodel.h"

Link::Link(const QString &page, QNetworkReply* reply)
{
    this->page = page;
    this->reply = reply;
    percentage = 0;
}

QString Link::getPage() const
{
    return page;
}

int Link::getPercentage() const
{
    return percentage;
}

LinkModel::LinkModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void LinkModel::addLink(Link* link)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    list_links << link;
    endInsertRows();
}

int LinkModel::rowCount(const QModelIndex & parent) const {
    return list_links.count();
}

QVariant LinkModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= list_links.count())
        return QVariant();

    Link* link = list_links.at(index.row());
    if (role == PageRole)
        return link->getPage();
    else if (role == PercentageRole)
        return link->getPercentage();
    return QVariant();
}

void LinkModel::setPercentage(QNetworkReply *reply, int percentage)
{
    Link* link;
    for (int i = 0; i < list_links.size(); ++i)
    {
        link = list_links.at(i);
        if(link->reply == reply)
        {
            link->percentage = percentage;
            dataChanged(index(i), index(i));
            return;
        }
    }
}

void LinkModel::clearList()
{
    beginResetModel();
    foreach (Link* link, list_links)
    {
        delete link;
    }
    list_links.clear();
    endResetModel();
}

QHash<int, QByteArray> LinkModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PageRole] = "page";
    roles[PercentageRole] = "percentage";
    return roles;
}


