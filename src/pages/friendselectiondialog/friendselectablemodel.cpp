#include "friendselectablemodel.h"
#include "../../wfc/client/ChatClient.h"


FriendSelectableModel::FriendSelectableModel(QObject *parent)
    : QAbstractListModel{parent}
{
    reloadData();
}

void FriendSelectableModel::reloadData()
{
    std::list<std::string> fIds = WFCLib::ChatClient::Instance()->getMyFriendList(false);
    foreach (auto fid, fIds) {
        friendIds << fid.c_str();
        checked << false;
    }
}

QStringList FriendSelectableModel::getCheckedIds()
{
    QStringList result;
    for (int i = 0; i < friendIds.count(); ++i) {
        if(checked.at(i)) {
            result << friendIds.at(i);
        }
    }
    return result;
}

void FriendSelectableModel::uncheckId(const QString &id)
{
    for (int i = 0; i < friendIds.count(); ++i) {
        if(friendIds.at(i) == id) {
            checked[i] = false;
            emit dataChanged(index(i), index(i), {Qt::CheckStateRole});
            break;
        }
    }
}

int FriendSelectableModel::rowCount(const QModelIndex &parent) const
{
    return friendIds.count();
}

QVariant FriendSelectableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::UserRole) {
        return friendIds.at(index.row());
    } else if(role == Qt::CheckStateRole) {
        return checked[index.row()] ? Qt::Checked : Qt::Unchecked;
    }
    return QVariant();
}

bool FriendSelectableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole) {
        checked[index.row()] = value.toBool();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}
