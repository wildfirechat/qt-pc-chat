#ifndef CHATLISTMODEL_H
#define CHATLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "../../wfc/client/ChatClient.h"


class ChatListModel : public QAbstractListModel, public WFCLib::GeneralVoidCallback
{
    Q_OBJECT
public:
    explicit ChatListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void reloadData();

    const WFCLib::ConversationInfo infoAt(const QModelIndex &index) const;

    void setTopAt(const QModelIndex &index, bool isTop);
    void setSilentAt(const QModelIndex &index, bool isSilent);
    void setReadAt(const QModelIndex &index, bool isRead);
    void removeAt(const QModelIndex &index);

    const QModelIndex indexOf(const WFCLib::ConversationInfo &conversationInfo) const;

    const QModelIndex focusConversaton(const WFCLib::Conversation &conversation);

private slots:
    void onImageLoaded(const QString &url, int row, int column);

private:
    QVector<WFCLib::ConversationInfo> conversations;
    QVector<WFCLib::ConversationInfo> emptyConversations;

    // GeneralVoidCallback interface
public:
    void onSuccess(int dataType);
    void onFailure(int dataType, int errorCode);
};

#endif // CHATLISTMODEL_H
