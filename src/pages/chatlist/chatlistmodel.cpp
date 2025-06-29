#include "chatlistmodel.h"
#include <QVector>

ChatListModel::ChatListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    std::list<WFCLib::ConversationInfo> ls = WFCLib::ChatClient::Instance()->getConversationInfos({WFCLib::Single_Type, WFCLib::Group_Type, WFCLib::Channel_Type}, {0});
    conversations = QVector<WFCLib::ConversationInfo>(ls.begin(), ls.end());
}

int ChatListModel::rowCount(const QModelIndex &parent) const
{
    return conversations.size();
}

QVariant ChatListModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::UserRole) {
        return QVariant::fromValue(conversations.at(index.row()));
    }

    return QVariant();
}

void ChatListModel::reloadData()
{
    beginResetModel();
    std::list<WFCLib::ConversationInfo> ls = WFCLib::ChatClient::Instance()->getConversationInfos({WFCLib::Single_Type, WFCLib::Group_Type, WFCLib::Channel_Type}, {0});
    conversations = QVector<WFCLib::ConversationInfo>(ls.begin(), ls.end());

    if(!emptyConversations.empty()) {
        // 从后向前遍历并移除符合条件的元素
        for (int i = emptyConversations.size() - 1; i >= 0; --i) {
            WFCLib::ConversationInfo emptyConv = emptyConversations.at(i);

            bool exist = false;
            for (const auto& item : conversations) {
                if (item.conversation == emptyConv.conversation) {
                    exist = true;
                    break;
                }
            }

            if (exist) {
                emptyConversations.remove(i); // 安全移除当前索引的元素
            }
        }

        //归并
        int insertPos = 0;
        while (insertPos < conversations.size() && conversations[insertPos].isTop > 0) {
            insertPos++;
        }
        for (const auto& item : emptyConversations) {
            conversations.insert(insertPos++, item);
        }
    }

    endResetModel();
}

const WFCLib::ConversationInfo ChatListModel::infoAt(const QModelIndex &index) const
{
    return conversations.at(index.row());
}

void ChatListModel::setTopAt(const QModelIndex &index, bool isTop)
{
    WFCLib::ConversationInfo info = conversations.at(index.row());
    WFCLib::ChatClient::Instance()->setConversationTop(info.conversation, isTop, this, 0);
}

void ChatListModel::setSilentAt(const QModelIndex &index, bool isSilent)
{
    WFCLib::ConversationInfo info = conversations.at(index.row());
    WFCLib::ChatClient::Instance()->setConversationSilent(info.conversation, isSilent, this, 0);
}

void ChatListModel::setReadAt(const QModelIndex &index, bool isRead)
{
    WFCLib::ConversationInfo info = conversations.at(index.row());
    if(isRead) {
        WFCLib::ChatClient::Instance()->clearUnreadStatus(info.conversation);
    } else {
        WFCLib::ChatClient::Instance()->markAsUnread(info.conversation, true);
    }
    reloadData();
}

void ChatListModel::removeAt(const QModelIndex &index)
{
    WFCLib::ConversationInfo info = conversations.at(index.row());
    WFCLib::ChatClient::Instance()->removeConversation(info.conversation, true);
    for (int i = emptyConversations.size() - 1; i >= 0; --i) {
        WFCLib::ConversationInfo &emptyConv = emptyConversations[i];
        if (info.conversation == emptyConv.conversation) {
            emptyConversations.remove(i);
            break;
        }
    }
    reloadData();
}

const QModelIndex ChatListModel::indexOf(const WFCLib::ConversationInfo &conversationInfo) const
{
    for(int i = 0; i < conversations.length(); i++) {
        WFCLib::ConversationInfo conv = conversations.at(i);
        if(conv.conversation.conversationType == conversationInfo.conversation.conversationType && conv.conversation.target == conversationInfo.conversation.target && conv.conversation.line == conversationInfo.conversation.line) {
            return createIndex(i, 0);
        }
    }
    return createIndex(0, 0);
}

const QModelIndex ChatListModel::focusConversaton(const WFCLib::Conversation &conversation)
{
    for(int i = 0; i < conversations.length(); i++) {
        WFCLib::ConversationInfo conv = conversations.at(i);
        if(conv.conversation.conversationType == conversation.conversationType && conv.conversation.target == conversation.target && conv.conversation.line == conversation.line) {
            return createIndex(i, 0);
        }
    }
    WFCLib::ConversationInfo conversationInfo = WFCLib::ChatClient::Instance()->getConversationInfo(conversation);
    emptyConversations.insert(0, conversationInfo);

    int insertPos = 0;
    while (insertPos < conversations.size() && conversations[insertPos].isTop > 0) {
        insertPos++;
    }

    conversations.insert(insertPos, conversationInfo);
    insertRow(insertPos);

    return createIndex(insertPos, 0);
}

void ChatListModel::onImageLoaded(const QString &url, int row, int column)
{
    if(row >= 0) {
        QModelIndex topLeft = index(row, 0);
        QModelIndex bottomRight = index(row, 0);
        emit dataChanged(topLeft, bottomRight);
    }
}

void ChatListModel::onSuccess(int dataType)
{
    reloadData();
}

void ChatListModel::onFailure(int dataType, int errorCode)
{
    reloadData();
}
