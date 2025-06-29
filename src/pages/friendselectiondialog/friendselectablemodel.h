#ifndef FRIENDSELECTABLEMODEL_H
#define FRIENDSELECTABLEMODEL_H

#include <QAbstractListModel>

class FriendSelectableModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FriendSelectableModel(QObject *parent = nullptr);
    void reloadData();
    QStringList getCheckedIds();
    void uncheckId(const QString &id);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QVector<QString> friendIds;
    QVector<bool> checked;
};

#endif // FRIENDSELECTABLEMODEL_H
