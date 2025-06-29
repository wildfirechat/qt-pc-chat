// contactlistmodel.cpp
#include "contactlistmodel.h"
#include "../../wfc/client/ChatClient.h"
#include <QStyledItemDelegate>
#include <QPainter>
#include "../utility/defaultportraitfactory.h"
#include <QDebug>
#include <QAbstractListModel>
#include <QVector>
#include "../../tools/imageloader.h"

ContactListModel::ContactListModel(QObject *parent)
    : QAbstractItemModel(parent) {
    connect(ImageLoader::getInstance(), &ImageLoader::imageLoaded, this, &ContactListModel::onImageLoaded);

    loadData();
}

void ContactListModel::loadData()
{
    foreach (auto fr, WFCLib::ChatClient::Instance()->getIncommingFriendRequest()) {
        friendRequests.push_back(fr);
    }

    foreach (auto fid, WFCLib::ChatClient::Instance()->getMyFriendList(true)) {
        friendIdList.push_back(fid);
    }

    foreach (auto fg, WFCLib::ChatClient::Instance()->getFavGroups()) {
        favGroups.push_back(fg);
    }

    foreach (auto lc, WFCLib::ChatClient::Instance()->getListenedChannels()) {
        listenedChannels.push_back(lc);
    }
}

int ContactListModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return groupList.size();
    } else if (parent.internalId() == 0) {
        if(parent.row() == 0) { //new friend
            return friendRequests.size();
        } else if(parent.row() == 1) { //saved group
            return favGroups.size();
        } else if(parent.row() == 2) { //channel
            return listenedChannels.size();
        } else if(parent.row() == 3) { //org
            return 0;
        } else if(parent.row() == 4) { //contact
            return friendIdList.size();
        }
    }
    return 0;
}

int ContactListModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (!index.parent().isValid()) {
            return QString::fromStdString(groupList[index.row()]);
        } else {
            QModelIndex parent = index.parent();
            if(parent.row() == 0) { //new friend
                auto userInfo = WFCLib::ChatClient::Instance()->getUserInfo(
                    friendRequests.at(index.row()).target, false);
                return QString::fromStdString(userInfo.getReadableName());
            } else if(parent.row() == 1) { //saved group
                auto groupInfo = WFCLib::ChatClient::Instance()->getGroupInfo(favGroups.at(index.row()), false);
                QString displayText = groupInfo.name.empty()
                                          ? groupInfo.target.c_str()
                                          : groupInfo.name.c_str();
                return displayText;
            } else if(parent.row() == 2) { //channel
                auto channelInfo = WFCLib::ChatClient::Instance()->getChannelInfo(listenedChannels.at(index.row()), false);
                QString displayText = channelInfo.name.c_str();
                return displayText;
            } else if(parent.row() == 3) { //org
                return 0;
            } else if(parent.row() == 4) { //contact
                auto userInfo = WFCLib::ChatClient::Instance()->getUserInfo(
                    friendIdList.at(index.row()), false);
                return QString::fromStdString(userInfo.getReadableName());
            }
        }
    } else if (role == Qt::UserRole) {
        if (!index.parent().isValid()) {
            return QString::fromStdString(groupList[index.row()]);
        } else {
            QModelIndex parent = index.parent();
            if(parent.row() == 0) { //new friend
                return QString::fromStdString(friendRequests.at(index.row()).target);
            } else if(parent.row() == 1) { //saved group
                return QString::fromStdString(favGroups.at(index.row()));
            } else if(parent.row() == 2) { //channel
                return QString::fromStdString(listenedChannels.at(index.row()));
            } else if(parent.row() == 3) { //org
                return 0;
            } else if(parent.row() == 4) { //contact
                return QString::fromStdString(friendIdList.at(index.row()));
            }
        }
    } else if (role == Qt::UserRole + 1) {
        QModelIndex parent = index.parent();
        if(parent.row() == 0) { //new friend
            auto userInfo = WFCLib::ChatClient::Instance()->getUserInfo(
                friendRequests.at(index.row()).target, false);
            return QString::fromStdString(userInfo.portrait);
        } else if(parent.row() == 1) { //saved group
            auto groupInfo = WFCLib::ChatClient::Instance()->getGroupInfo(favGroups.at(index.row()), false);
            QString displayText = groupInfo.portrait.c_str();
            return displayText;
        } else if(parent.row() == 2) { //channel
            auto channelInfo = WFCLib::ChatClient::Instance()->getChannelInfo(listenedChannels.at(index.row()), false);
            QString displayText = channelInfo.portrait.c_str();
            return displayText;
        } else if(parent.row() == 3) { //org
            return 0;
        } else if(parent.row() == 4) { //contact
            auto userInfo = WFCLib::ChatClient::Instance()->getUserInfo(
                friendIdList.at(index.row()), false);
            return QString::fromStdString(userInfo.portrait);
        }
    } else if (role == Qt::UserRole + 2) {
        if(index.row() == 0) { //new friend
            return QString::number(friendRequests.size());
        } else if(index.row() == 1) { //saved group
            return QString::number(favGroups.size());
        } else if(index.row() == 2) { //channel
            return QString::number(listenedChannels.size());
        } else if(index.row() == 3) { //org
            return QString::number(-1);
        } else if(index.row() == 4) { //contact
            return QString::number(friendIdList.size());
        }
    }

    return QVariant();
}

QModelIndex ContactListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid()) {
        return createIndex(row, column, (quintptr)0);
    } else if (parent.internalId() == 0) {
        return createIndex(row, column, (quintptr)(parent.row() + 1));
    }
    return QModelIndex();
}

QModelIndex ContactListModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    if (index.internalId() == 0)
        return QModelIndex();
    else {
        return createIndex(index.internalId() - 1, 0, (quintptr)0);
    }
}

void ContactListModel::onImageLoaded(const QString &url, int row, int column, const QString &prefix) {
    if(row >= 0) {
        QModelIndex topLeft = index(row, 0);
        QModelIndex bottomRight = index(row, 0);
        emit dataChanged(topLeft, bottomRight);
    }
}
