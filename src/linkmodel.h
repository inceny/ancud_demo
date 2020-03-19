#include <QAbstractListModel>
#include <QStringList>
#include <QNetworkReply>

class Link
{
public:
    Link(const QString &page, QNetworkReply* reply);

    QString getPage() const;
    int getPercentage() const;
    int percentage;
    QNetworkReply* reply;

private:
    QString page;
};

class LinkModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum LinkRoles {
        PageRole = Qt::UserRole + 1,
        PercentageRole
    };

    LinkModel(QObject *parent = 0);
    void addLink(Link *link);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void setPercentage(QNetworkReply *reply, int percentage);
    void clearList();

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Link*> list_links;
};


