#ifndef CHATLISTWIDGET_H
#define CHATLISTWIDGET_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>
#include "chatlistmodel.h"
#include "../../wfc/client/ChatClient.h"


class ChatListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatListWidget(QWidget *parent = nullptr);
    void chatToUser(const QString &userId);
    void chatToConversaton(const WFCLib::Conversation &conversation);

signals:
    void itemSelected(const WFCLib::Conversation &itemId);

private slots:
    void onItemClicked(const QModelIndex &index);
    void onReceiveMessages(const std::list<WFCLib::Message> &messages, bool hasMore);
    void onUserSettingUpdated();
    void onGroupInfoUpdated(const std::list<WFCLib::GroupInfo> &groupInfos);
    void onUserInfoUpdated(const std::list<WFCLib::UserInfo> &userInfos);
    void onUnreadStatusCleared(const WFCLib::Conversation &conversation);
    void onMessageCleared(const WFCLib::Conversation &conversation, int64_t before);

private:
    void setupUI();
    void reloadData();

    QListView *listView;
    ChatListModel *model;

    QTimer *scrollEndTimer;
};

#endif // CHATLISTWIDGET_H
