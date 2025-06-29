// contactlistmodel.h
#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include <QAbstractItemModel>
#include "../../wfc/client/ChatClient.h"



class ContactListModel : public QAbstractItemModel {
    Q_OBJECT

public slots:
    void onImageLoaded(const QString &url, int row, int column, const QString &prefix);
    void loadData();

public:
    ContactListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

private:
    // 分组数据
    std::vector<std::string> groupList = {"新朋友", "群聊", "频道", "组织架构", "联系人"};

    std::vector<WFCLib::FriendRequest> friendRequests;
    std::vector<std::string> friendIdList;
    std::vector<std::string> listenedChannels;
    std::vector<std::string> favGroups;
};

#endif // CONTACTLISTMODEL_H
