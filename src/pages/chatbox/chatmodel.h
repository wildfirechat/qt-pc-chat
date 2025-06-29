#ifndef CHATMODEL_H
#define CHATMODEL_H


#include <QAbstractListModel>
#include "messagedata.h"


class ChatModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit ChatModel(QObject *parent = nullptr);

    // 实现必要的方法
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // 添加消息
    int addMessage(const WFCLib::Message &message);
    int addMessages(const std::list<WFCLib::Message> &messages, bool sort = true);
    void onMessageMediaUploaded(long messageId, const std::string &remoteUrl);
    void clear();

    void setFirstUnreadMessageId(long newFirstUnreadMessageId);

    void setMessagePlaying(long messageId, bool playing);
    void refreshMessage(long messageId);

    long getTopMessageId(bool top);
    int64_t getTopMessageUid(bool top);

private:
    QList<MessageData> m_messages;
    long firstUnreadMessageId;
};


#endif // CHATMODEL_H
