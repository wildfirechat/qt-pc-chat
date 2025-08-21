#include "callwebinterface.h"
#include <QDebug>


CallWebInterface::CallWebInterface(const WFCLib::Conversation conversation, const QList<QString> receivers, bool audioOnly, QObject *parent): QObject{parent}, mConversation(conversation), mReceivers(receivers), mAudioOnly(audioOnly) {

}

QString CallWebInterface::fromWeb(const QString &message) {
    qDebug() << "receive web:" << message;
    return QString("From C++: %1").arg(message);
}

void CallWebInterface::onReady()
{
    emit startCall(mConversation.conversationType, QString::fromStdString(mConversation.target), mConversation.line, mReceivers, mAudioOnly);
}

void CallWebInterface::sendConferenceRequest(int requestId, long long sessionId, const QString &roomId, const QString &request, bool advance, const QString &data) {
    WFCLib::ChatClient::Instance()->sendConferenceRequest(sessionId, roomId.toStdString(), request.toStdString(), advance, data.toStdString(), this, requestId);
}

QString CallWebInterface::sendMessage(int requestId, int conversationType, const QString &target, int line, const QString &content, const QList<QString> toUsers) {
    WFCLib::MessagePayload payload;
    payload.fromJson(content.toStdString());
    WFCLib::MessageContent *msgCont = WFCLib::ChatClient::Instance()->getContent(payload);

    std::list<std::string> stdToUsers;
    for(auto t : toUsers) {
        stdToUsers.push_back(t.toStdString());
    }
    WFCLib::Message message = WFCLib::ChatClient::Instance()->sendMessage(mConversation, *msgCont, stdToUsers, 0, this, requestId);
    delete msgCont;

    return QString::fromStdString(message.toJson());
}

void CallWebInterface::onCallStateChanged(const QString &callId, int newState) {

}

QString CallWebInterface::getUserInfo(const QString &userId) {
    WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(userId.toStdString());
    return QString::fromStdString(userInfo.toJson());
}

QString CallWebInterface::getGroupInfo(const QString &groupId) {
    WFCLib::GroupInfo groupInfo = WFCLib::ChatClient::Instance()->getGroupInfo(groupId.toStdString());
    return QString::fromStdString(groupInfo.toJson());
}

void CallWebInterface::onSuccess(int dataType, const std::string &value)
{
    emit sendConferenceResponse(dataType, 0, QString::fromStdString(value));
}

//onFailure有可能是发送conferenceRequet返回的错误，也有可能是sendMessage返回的错误。
//区分是在请求的时候dataType做了处理，一个为正值，一个为负值，这样就区分开了。
void CallWebInterface::onFailure(int dataType, int errorCode)
{
    if(dataType>0) {
        emit sendMessageResult(dataType, errorCode, 0, 0);
    } else {
        emit sendConferenceResponse(-dataType, errorCode, QString());
    }
}

void CallWebInterface::onSuccess(int dataType, int64_t messageUid, int64_t timestamp)
{
    emit sendMessageResult(dataType, 0, messageUid, timestamp);
}

void CallWebInterface::onPrepared(int dataType, int messageId, int64_t timestamp)
{
    
}

void CallWebInterface::onProgress(int dataType, int uploaded, int total)
{
    
}

void CallWebInterface::onUploaded(int dataType, const std::string &remoteUrl)
{
    
}
