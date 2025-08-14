#ifndef CALLWEBINTERFACE_H
#define CALLWEBINTERFACE_H

#include <QObject>
#include "../../wfc/client/ChatClient.h"

// 1. 定义交互类（供网页调用的Qt对象）
class CallWebInterface : public QObject, public WFCLib::GeneralStringCallback, public WFCLib::WFSendMessageCallback
{
    Q_OBJECT  // 必须添加，支持元对象系统
public:
    explicit CallWebInterface(const WFCLib::Conversation conversation, const QList<QString> receivers, bool audioOnly, QObject *parent = nullptr);

    //下面是Web调用的API
    Q_INVOKABLE QString fromWeb(const QString &message);
    Q_INVOKABLE void onReady();
    Q_INVOKABLE void sendConferenceRequest(int requestId, long long sessionId, const QString &roomId, const QString &request, bool advance, const QString &data);
    Q_INVOKABLE QString sendMessage(int requestId, int conversationType, const QString &target, int line, const QString &content, const QList<QString> toUsers);
    Q_INVOKABLE void onCallStateChanged(const QString &callId, int newState);
    Q_INVOKABLE QString getUserInfo(const QString &userId);
    Q_INVOKABLE QString getGroupInfo(const QString &groupId);

signals:
    // 下面是发送给网页的请求.
    void sendToWeb(const QString &message);
    void startCall(int conversationType, const QString &targetId, int line, const QList<QString> &receivers, bool audioOnly);
    void sendMessageResult(int requestId, int errorcode, long long messageUid, long long timestamp);
    void sendConferenceResponse(int requestId, int errorCode, const QString &response);
    void onConferenceEvent(const QString &event);

public:
    virtual void onSuccess(int dataType, const std::string &value);
    virtual void onFailure(int dataType, int errorCode);

    virtual void onSuccess(int dataType, int64_t messageUid, int64_t timestamp);
    virtual void onPrepared(int dataType, int messageId, int64_t timestamp);
    virtual void onProgress(int dataType, int uploaded, int total);
    virtual void onUploaded(int dataType, const std::string &remoteUrl);

private:
    WFCLib::Conversation mConversation;
    QList<QString> mReceivers;
    bool mAudioOnly;
};
#endif // CALLWEBINTERFACE_H
