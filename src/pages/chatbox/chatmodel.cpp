#include "chatmodel.h"
#include "../../wfc/client/ChatClient.h"


int ChatModel::rowCount(const QModelIndex &parent) const {
    return m_messages.size();
}

QVariant ChatModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_messages.size())
        return QVariant();

    const MessageData msg = m_messages.at(index.row());
    if (role == Qt::UserRole)  // 返回消息对象
        return QVariant::fromValue(msg);

    return QVariant();
}

int ChatModel::addMessage(const WFCLib::Message &msg) {
    return addMessages({msg}, false);
}

int ChatModel::addMessages(const std::list<WFCLib::Message> &msgs, bool sort)
{
    bool update = false;
    int insertCount = msgs.size();
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());
    foreach (auto msg, msgs) {
        bool duplate = false;
        for (int i = 0; i < m_messages.size(); ++i) {
            MessageData &m = m_messages[i];
            if((m.message.messageId>0 && m.message.messageId == msg.messageId) || (m.message.messageUid > 0 && m.message.messageUid == msg.messageUid)) {
                m.message = msg;
                duplate = true;
                update = true;
                insertCount--;
                break;
            }
        }
        if(!duplate) {
            MessageData md(msg);
            m_messages.append(md);
        }
    }

    if(sort) {
        std::sort(m_messages.begin(), m_messages.end(), [](const MessageData &p1, const MessageData &p2){
            return p1.message.timestamp < p2.message.timestamp;
        });
    }

    const MessageData *prev = nullptr;
    for (int i = 0; i < m_messages.size(); ++i) {
        MessageData &current = m_messages[i];

        if (prev == nullptr || current.message.timestamp - prev->message.timestamp > 180*1000) {
            current.showTime = true;
        } else {
            current.showTime = false;
        }
        prev = &current;
    }
    endInsertRows();
    if(update) {
        emit dataChanged(createIndex(0, 0), createIndex(m_messages.size()-1, 0));
    }

    return insertCount;
}

void ChatModel::onMessageMediaUploaded(long messageId, const std::string &remoteUrl)
{
    for(QList<MessageData>::iterator it = m_messages.begin(); it != m_messages.end(); ++it) {
        if(it->message.messageId == messageId) {
            ((WFCLib::MediaMessageContent *)(it->message.content))->remoteUrl = remoteUrl;
        }
    }
}

void ChatModel::clear()
{
    beginResetModel();
    m_messages.clear();
    endResetModel();
}

void ChatModel::setFirstUnreadMessageId(long newFirstUnreadMessageId)
{
    firstUnreadMessageId = newFirstUnreadMessageId;
    if(firstUnreadMessageId < 0) firstUnreadMessageId = 0;
}

void ChatModel::setMessagePlaying(long messageId, bool playing)
{
    for(QList<MessageData>::iterator it = m_messages.begin(); it != m_messages.end(); ++it) {
        if(it->message.messageId == messageId) {
            it->voicePlaying = playing;
        } else {
            it->voicePlaying = false;
        }
    }
}

void ChatModel::refreshMessage(long messageId)
{
    for (int row = 0; row < m_messages.length(); ++row) {
        if(m_messages.at(row).message.messageId == messageId) {
            emit dataChanged(index(row), index(row), {Qt::DisplayRole});
        }
    }
}

long ChatModel::getTopMessageId(bool top)
{
    long messageId = 0;
    int64_t maxTime = !top ? 0 : LONG_MAX;
    for(auto md : m_messages) {
        if(md.message.timestamp < maxTime && top) {
            maxTime = md.message.timestamp;
            messageId = md.message.messageId;
        } else if(md.message.timestamp > maxTime && !top) {
            maxTime = md.message.timestamp;
            messageId = md.message.messageId;
        }
    }
    return messageId;
}

int64_t ChatModel::getTopMessageUid(bool top)
{
    long messageUid = 0;
    int64_t maxTime = !top ? 0 : LONG_MAX;
    for(auto md : m_messages) {
        if(md.message.timestamp < maxTime && top && md.message.messageUid > 0) {
            maxTime = md.message.timestamp;
            messageUid = md.message.messageUid;
        } else if(md.message.timestamp > maxTime && !top && md.message.messageUid > 0) {
            maxTime = md.message.timestamp;
            messageUid = md.message.messageUid;
        }
    }
    return messageUid;
}

ChatModel::ChatModel(QObject *parent) : QAbstractListModel(parent)
{}
