#include "avenginekitproxy.h"
#include "voipwebviewwidget.h"
#include "../../wfc/enum/WFEnums.h"
#include <QJsonArray>
#include <QDateTime>
#include <QApplication>
#include <QDebug>

AvEngineKitProxy* AvEngineKitProxy::s_instance = nullptr;

AvEngineKitProxy::AvEngineKitProxy(QObject *parent)
    : QObject(parent)
    , m_client(nullptr)
    , m_conference(false)
    , m_inviteMessageUid(0)
    , m_isSupportVoip(true)
    , m_hasMicrophone(true)
    , m_hasSpeaker(true)
    , m_hasWebcam(true)
    , m_voipWebview(nullptr)
    , m_debug(false)
    , m_deviceCheckTimer(new QTimer(this))
{
    // 检查设备可用性
    checkDevices();

    // 定期检查设备状态
    m_deviceCheckTimer->setInterval(5000); // 5秒检查一次
    connect(m_deviceCheckTimer, &QTimer::timeout, this, &AvEngineKitProxy::checkDevices);
    m_deviceCheckTimer->start();
}

AvEngineKitProxy::~AvEngineKitProxy()
{
    // 析构时不需要移除回调，因为使用的是信号槽
}

AvEngineKitProxy* AvEngineKitProxy::instance()
{
    if (!s_instance) {
        s_instance = new AvEngineKitProxy();
    }
    return s_instance;
}

void AvEngineKitProxy::setup(WFCLib::ChatClient* client)
{
    if (m_client == client) {
        qDebug() << "re-setup, just ignore";
        return;
    }

    qDebug() << "avenginekitproxy setup";

    if (m_client) {
        // 断开之前的连接
        disconnect(m_client, &WFCLib::ChatClient::receiveMessages, this, &AvEngineKitProxy::onReceiveMessages);
    }

    m_client = client;
    if (m_client) {
        // 使用信号槽连接消息接收
        connect(m_client, &WFCLib::ChatClient::receiveMessages, this, &AvEngineKitProxy::onReceiveMessages);
        // 连接会议事件信号（如果存在）
        // connect(m_client, &WFCLib::ChatClient::conferenceEvent, this, &AvEngineKitProxy::onConferenceEvent);
    }
}

void AvEngineKitProxy::setVoipWebview(VoipWebViewWidget* webview)
{
    m_voipWebview = webview;

    if (!webview) {
        // 通话结束，清理状态
        if (m_onVoipCallStatusCallback) {
            m_onVoipCallStatusCallback(m_conversation, false);
        }

        m_conversation = WFCLib::Conversation();
        m_queueEvents.clear();

        if (m_conference && m_client) {
            m_client->quitChatroom(m_callId.toStdString());
            m_conference = false;
        }

        m_callId.clear();
        m_participants.clear();
        m_inviteMessageUid = 0;
    } else {
        // 处理队列中的事件
        processQueuedEvents();
    }
}

void AvEngineKitProxy::startCall(const WFCLib::Conversation& conversation, bool audioOnly,
                                const QStringList& participants, const QString& callExtra)
{
    if (m_voipWebview) {
        qDebug() << "voip call is ongoing";
        if (m_onVoipCallErrorCallback) {
            m_onVoipCallErrorCallback(-1);
        }
        return;
    }

    qDebug() << QString("startCall speaker、microphone、webcam检测结果分别为：%1, %2, %3，如果不全为true，请检查硬件设备是否正常，否则通话可能存在异常")
                .arg(m_hasSpeaker).arg(m_hasMicrophone).arg(m_hasWebcam);

    if (!m_isSupportVoip || !m_hasSpeaker || !m_hasMicrophone) {
        qDebug() << "not support voip" << m_isSupportVoip << m_hasSpeaker << m_hasMicrophone << m_hasWebcam;
        if (m_onVoipCallErrorCallback) {
            m_onVoipCallErrorCallback(-2);
        }
        return;
    }

    if (!m_client) {
        qDebug() << "ChatClient not initialized";
        return;
    }

    WFCLib::UserInfo selfUserInfo = m_client->getUserInfo(m_client->getCurrentUserId());
    QStringList filteredParticipants;

    // 过滤掉自己
    for (const QString& uid : participants) {
        if (uid != QString::fromStdString(selfUserInfo.uid)) {
            filteredParticipants.append(uid);
        }
    }

    QString callId = QString::fromStdString(conversation.target) + QString::number(rand() % 10000);
    m_conversation = conversation;
    m_participants = filteredParticipants;
    m_callId = callId;

    QJsonObject args;
    args["conversation"] = QJsonObject{
        {"type", conversation.conversationType},
        {"target", QString::fromStdString(conversation.target)},
        {"line", conversation.line}
    };
    args["audioOnly"] = audioOnly;
    args["callId"] = callId;
    args["selfUserInfo"] = QJsonObject{
        {"uid", QString::fromStdString(selfUserInfo.uid)},
        {"displayName", QString::fromStdString(selfUserInfo.displayName)}
    };

    QJsonArray participantsArray;
    for (const QString& participant : filteredParticipants) {
        participantsArray.append(participant);
    }
    args["participants"] = participantsArray;
    args["callExtra"] = callExtra;

    showCallUI(conversation, false, QJsonObject{
        {"event", "startCall"},
        {"args", args}
    });
}

void AvEngineKitProxy::startConference(const QString& callId, bool audioOnly, const QString& pin,
                                     const QString& host, const QString& title, const QString& desc,
                                     bool audience, bool advance, bool record,
                                     const QJsonObject& extra, const QJsonObject& callExtra,
                                     bool muteAudio, bool muteVideo)
{
    if (m_voipWebview) {
        qDebug() << "voip call is ongoing";
        if (m_onVoipCallErrorCallback) {
            m_onVoipCallErrorCallback(-1);
        }
        return;
    }

    if (!m_isSupportVoip || !m_hasSpeaker || !m_hasMicrophone) {
        qDebug() << "not support voip" << m_isSupportVoip << m_hasSpeaker;
        if (m_onVoipCallErrorCallback) {
            m_onVoipCallErrorCallback(-2);
        }
        return;
    }

    if (!m_client) {
        qDebug() << "ChatClient not initialized";
        return;
    }

    QString actualCallId = callId.isEmpty() ?
        QString::fromStdString(m_client->getCurrentUserId()) + QString::number(rand() % 10000) : callId;

    m_callId = actualCallId;
    m_conversation = WFCLib::Conversation();
    m_conference = true;

    // 加入会议聊天室
    // m_client->joinChatroom(actualCallId.toStdString(), [actualCallId](int errorCode) {
    //     if (errorCode == 0) {
    //         qDebug() << "join conference chatRoom success" << actualCallId;
    //     } else {
    //         qDebug() << "join conference chatRoom fail" << actualCallId << errorCode;
    //     }
    // });

    WFCLib::UserInfo selfUserInfo = m_client->getUserInfo(m_client->getCurrentUserId());

    QJsonObject args;
    args["audioOnly"] = audioOnly;
    args["callId"] = actualCallId;
    args["pin"] = pin.isEmpty() ? QString::number(rand() % 1000000) : pin;
    args["host"] = host;
    args["title"] = title;
    args["desc"] = desc;
    args["audience"] = audience;
    args["advance"] = advance;
    args["record"] = record;
    args["selfUserInfo"] = QJsonObject{
        {"uid", QString::fromStdString(selfUserInfo.uid)},
        {"displayName", QString::fromStdString(selfUserInfo.displayName)}
    };
    args["extra"] = extra;
    args["callExtra"] = callExtra;
    args["muteAudio"] = muteAudio;
    args["muteVideo"] = muteVideo;

    showCallUI(WFCLib::Conversation(), true, QJsonObject{
        {"event", "startConference"},
        {"args", args}
    });
}

void AvEngineKitProxy::joinConference(const QString& callId, bool audioOnly, const QString& pin,
                                    const QString& host, const QString& title, const QString& desc,
                                    bool audience, bool advance, bool muteAudio, bool muteVideo,
                                    const QJsonObject& extra, const QJsonObject& callExtra)
{
    if (m_voipWebview) {
        qDebug() << "voip call is ongoing";
        if (m_onVoipCallErrorCallback) {
            m_onVoipCallErrorCallback(-1);
        }
        return;
    }

    if (!m_isSupportVoip) {
        qDebug() << "not support voip" << m_isSupportVoip << m_hasSpeaker;
        if (m_onVoipCallErrorCallback) {
            m_onVoipCallErrorCallback(-2);
        }
        return;
    }

    if (!m_client) {
        qDebug() << "ChatClient not initialized";
        return;
    }

    m_conversation = WFCLib::Conversation();
    m_conference = true;
    m_callId = callId;

    // 加入会议聊天室
    // m_client->joinChatroom(callId.toStdString(), [callId](int errorCode) {
    //     if (errorCode == 0) {
    //         qDebug() << "join conference chatRoom success" << callId;
    //     } else {
    //         qDebug() << "join conference chatRoom fail" << callId << errorCode;
    //     }
    // });

    WFCLib::UserInfo selfUserInfo = m_client->getUserInfo(m_client->getCurrentUserId());

    QJsonObject args;
    args["audioOnly"] = audioOnly;
    args["callId"] = callId;
    args["pin"] = pin;
    args["host"] = host;
    args["title"] = title;
    args["desc"] = desc;
    args["audience"] = audience;
    args["advance"] = advance;
    args["muteAudio"] = muteAudio;
    args["muteVideo"] = muteVideo;
    args["selfUserInfo"] = QJsonObject{
        {"uid", QString::fromStdString(selfUserInfo.uid)},
        {"displayName", QString::fromStdString(selfUserInfo.displayName)}
    };
    args["extra"] = extra;
    args["callExtra"] = callExtra;

    showCallUI(WFCLib::Conversation(), true, QJsonObject{
        {"event", "joinConference"},
        {"args", args}
    });
}

void AvEngineKitProxy::setVoipCallErrorCallback(VoipCallErrorCallback callback)
{
    m_onVoipCallErrorCallback = callback;
}

void AvEngineKitProxy::setVoipCallStatusCallback(VoipCallStatusCallback callback)
{
    m_onVoipCallStatusCallback = callback;
}

void AvEngineKitProxy::emitToVoip(const QString& event, const QJsonObject& args)
{
    qDebug() << "emit to voip" << event << args;
    if (m_voipWebview) {
        QJsonObject data;
        data["event"] = event;
        data["args"] = args;

        // 延迟发送，等待音视频页面加载完成
        QTimer::singleShot(10000, [this, data]() {
            if (m_voipWebview) {
                QString jsonString = QJsonDocument(data).toJson(QJsonDocument::Compact);
                m_voipWebview->sendMessageToWeb(jsonString);

                if (data["event"].toString() == "startCall") {
                    qDebug() << "send startCall" << QDateTime::currentMSecsSinceEpoch();
                }
            }
        });
    } else if (!m_queueEvents.isEmpty() || event != "startCall") {
        // 将事件加入队列
        qDebug() << "queue voip event" << event;
        QueuedEvent queuedEvent;
        queuedEvent.event = event;
        queuedEvent.args = args;
        m_queueEvents.enqueue(queuedEvent);
    }
}

void AvEngineKitProxy::emitToMain(const QString& event, const QJsonObject& args)
{
    qDebug() << "emit to main" << event << args;
    voipWebviewEventListener(event, args);
}

void AvEngineKitProxy::onReceiveMessages(const std::list<WFCLib::Message>& messages, bool hasMore)
{
    qDebug() << "onReceiveMessages ...............";

    for (const WFCLib::Message& msg : messages) {
        if (isExpiredMessage(msg)) {
            qDebug() << "expired msg, ignore";
            continue;
        }

        if (isVoipMessage(msg.content->getPrototype()->getType())) {
            handleVoipMessage(msg);
            qDebug() << "onReceiveMessages voip message ...............";
        }
    }
}

void AvEngineKitProxy::onConferenceEvent(const QJsonObject& event)
{
    emitToVoip("conferenceEvent", event);
}

void AvEngineKitProxy::checkDevices()
{
  /*  // 检查音频设备
    QList<QAudioDeviceInfo> audioInputs = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QList<QAudioDeviceInfo> audioOutputs = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    m_hasMicrophone = !audioInputs.isEmpty();
    m_hasSpeaker = !audioOutputs.isEmpty();

    // 检查摄像头
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    m_hasWebcam = !cameras.isEmpty();

    // 检查WebRTC支持（简化检查）
    m_isSupportVoip = m_hasMicrophone && m_hasSpeaker*/;
}

void AvEngineKitProxy::processQueuedEvents()
{
    while (!m_queueEvents.isEmpty()) {
        QueuedEvent event = m_queueEvents.dequeue();
        qDebug() << "process queued event" << event.event;
        emitToVoip(event.event, event.args);
    }
}

void AvEngineKitProxy::showCallUI(const WFCLib::Conversation& conversation, bool isConference, const QJsonObject& options)
{
    QString type;
    if (isConference) {
        type = "conference";
    } else {
        type = (conversation.conversationType == WFCLib::Single_Type) ? "single" : "multi";
    }

    // 创建并显示通话窗口
    QStringList participants;
    if (options.contains("args") && options["args"].isObject()) {
        QJsonObject args = options["args"].toObject();
        if (args.contains("participants") && args["participants"].isArray()) {
            QJsonArray participantsArray = args["participants"].toArray();
            for (const QJsonValue& value : participantsArray) {
                participants.append(value.toString());
            }
        }
    }

    VoipWebViewWidget* callWidget = new VoipWebViewWidget(type, options);
    setVoipWebview(callWidget);
    callWidget->show();

    // 发送初始化事件
    emitToVoip(options["event"].toString(), options["args"].toObject());
}

bool AvEngineKitProxy::isVoipMessage(int messageType)
{
    // 根据实际的消息类型常量来判断
    return messageType == WFCLib::VOIP_CONTENT_TYPE_START ||
           messageType == WFCLib::VOIP_CONTENT_TYPE_END ||
           messageType == WFCLib::VOIP_CONTENT_TYPE_ACCEPT ||
           messageType == WFCLib::VOIP_CONTENT_TYPE_SIGNAL ||
           messageType == WFCLib::VOIP_CONTENT_TYPE_MODIFY ||
           messageType == WFCLib::VOIP_CONTENT_TYPE_ACCEPT_T ||
           messageType == WFCLib::VOIP_CONTENT_TYPE_ADD_PARTICIPANT ||
           messageType == WFCLib::VOIP_CONTENT_MUTE_VIDEO ||
           messageType == WFCLib::VOIP_CONTENT_JOIN_CALL_REQUEST ||
           messageType == WFCLib::VOIP_CONTENT_CONFERENCE_KICKOFF_MEMBER ||
           messageType == WFCLib::VOIP_CONTENT_CONFERENCE_CHANGE_MODE ||
           messageType == WFCLib::VOIP_CONTENT_CONFERENCE_COMMAND;
}

bool AvEngineKitProxy::isExpiredMessage(const WFCLib::Message& msg)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 msgTime = msg.timestamp;

    // 如果消息超过90秒，认为已过期
    return (now - msgTime) >= 90 * 1000;
}

void AvEngineKitProxy::handleVoipMessage(const WFCLib::Message& msg)
{
    if (m_voipWebview && m_conference &&
        msg.content->getPrototype()->getType() != WFCLib::VOIP_CONTENT_CONFERENCE_COMMAND) {
        qDebug() << "in conference, ignore all other msg";
        return;
    }

    int contentType = msg.content->getPrototype()->getType();

    // 处理通话邀请或添加参与者
    if (contentType == WFCLib::VOIP_CONTENT_TYPE_START ||
        contentType == WFCLib::VOIP_CONTENT_TYPE_ADD_PARTICIPANT) {

        if (m_voipWebview) {
            // 已在通话中，拒绝新的通话邀请
            if (m_onVoipCallErrorCallback) {
                m_onVoipCallErrorCallback(-1);
            }
            return;
        }
    }

    // 检查是否是相关的通话会话
    if ((msg.conversation.conversationType == WFCLib::Single_Type ||
         msg.conversation.conversationType == WFCLib::Group_Type ||
         (m_conference && msg.conversation.conversationType == WFCLib::Chatroom_Type))) {

        qDebug() << "receive voip message" << contentType << msg.messageUid;

        if (msg.direction == WFCLib::Direction_Send &&
            contentType != WFCLib::VOIP_CONTENT_TYPE_END &&
            contentType != WFCLib::VOIP_CONTENT_TYPE_ACCEPT) {
            return;
        }

        // 获取自己的用户信息
        WFCLib::UserInfo selfUserInfo = m_client->getUserInfo(m_client->getCurrentUserId());

        // 构建消息JSON对象发送到WebView
        QJsonObject msgJson;
        msgJson["messageUid"] = QString::number(msg.messageUid);
        msgJson["from"] = QString::fromStdString(msg.from);
        msgJson["timestamp"] = QString::number(msg.timestamp);
        msgJson["direction"] = msg.direction;

        QJsonObject conversationJson;
        conversationJson["type"] = msg.conversation.conversationType;
        conversationJson["target"] = QString::fromStdString(msg.conversation.target);
        conversationJson["line"] = msg.conversation.line;
        msgJson["conversation"] = conversationJson;

        QJsonObject contentJson;
        contentJson["type"] = contentType;
        // 这里需要根据具体的消息内容类型来序列化内容
        if(contentType == WFCLib::VOIP_CONTENT_TYPE_START) {
            auto content = static_cast<WFCLib::CallStartMessageContent *>(msg.content);
            if (content) {
                contentJson["callId"] = QString::fromStdString(content->callId);
                contentJson["audioOnly"] = content->audioOnly;

                QJsonArray targetIdsArray;
                for (const auto& targetId : content->targetIds) {
                    QString _targetId = QString::fromStdString(targetId);
                    targetIdsArray.append(_targetId);

                    m_participants.append(QString::fromStdString(msg.from));
                    if (_targetId != QString::fromStdString(selfUserInfo.uid)) {
                        m_participants.append(_targetId);
                    }
                }
                contentJson["targetIds"] = targetIdsArray;
                contentJson["connectTime"] = QString::number(content->connectTime);
                contentJson["endTime"] = QString::number(content->endTime);
                contentJson["status"] = content->status;

                contentJson["pin"] = QString::fromStdString(content->pin);
                contentJson["sdkType"] = content->sdkType;
            }
        // } else if (contentType == WFCLib::VOIP_CONTENT_TYPE_END) {
        //     auto content = static_cast<WFCLib::CallEndMessageContent *>(msg.content);
        //     if (content) {
        //         contentJson["callId"] = QString::fromStdString(content->callId);
        //         contentJson["reason"] = content->reason;
        //         contentJson["inviteMessageUid"] = QString::number(content->inviteMessageUid);
        //     }

        }else if(contentType == WFCLib::VOIP_CONTENT_TYPE_ACCEPT){
            auto content = static_cast<WFCLib::CallAnswerMessageContent *>(msg.content);
            if (content) {
                contentJson["callId"] = QString::fromStdString(content->callId);
                contentJson["audioOnly"] = content->audioOnly;
                contentJson["inviteMessageUid"] = QString::number(content->inviteMessageUid);
            }
        }else if(contentType == WFCLib::VOIP_CONTENT_TYPE_SIGNAL){
            auto content = static_cast<WFCLib::CallSignalMessageContent *>(msg.content);
            if (content) {
                contentJson["callId"] = QString::fromStdString(content->callId);
                contentJson["payload"] = QString::fromStdString(content->signalPayload);
            }
        }
        msgJson["messageContent"] = contentJson;

        msgJson["selfUserInfo"] = QJsonObject{
            {"uid", QString::fromStdString(selfUserInfo.uid)},
            {"displayName", QString::fromStdString(selfUserInfo.displayName)}
        };

        // 处理特定类型的消息
        if (contentType == WFCLib::VOIP_CONTENT_TYPE_START) {
            m_conversation = msg.conversation;
            // m_callId = content.callId; // 需要从消息内容中获取
            m_inviteMessageUid = msg.messageUid;

            // 添加参与者信息
            QJsonArray participantsArray;
            for (const QString& participant : m_participants) {
                participantsArray.append(participant);
            }
            msgJson["participants"] = participantsArray;

            if (!m_voipWebview) {
                showCallUI(msg.conversation, false, QJsonObject{
                    {"event", "message"},
                    {"args", msgJson}
                });
                return;
            }
        } else if (contentType == WFCLib::VOIP_CONTENT_TYPE_END) {
            // 检查callId是否匹配
            // if (content.callId != m_callId) return;

            m_conversation = WFCLib::Conversation();
            m_queueEvents.clear();
            m_callId.clear();
            m_inviteMessageUid = 0;
            m_participants.clear();
        }

        emitToVoip("message", msgJson);
    }
}

void AvEngineKitProxy::voipWebviewEventListener(const QString& event, const QJsonObject& args)
{
    qDebug() << "voipWebviewEventListener" << event << args;

    if (event == "voip-message") {
        sendVoipListener(event, args);
    } else if (event == "conference-request") {
        sendConferenceRequestListener(event, args);
    } else if (event == "update-call-start-message") {
        updateCallStartMessageContentListener(event, args);
    } else if (event == "pickGroupMembers") {
        pickGroupMembers(event, args);
    } else if (event == "inviteConferenceParticipant") {
        inviteConferenceParticipant(event, args);
    } else if (event == "didCallEndWithReason") {
        didCallEndWithReason(event, args);
    }
}

void AvEngineKitProxy::sendVoipListener(const QString& event, const QJsonObject& args)
{
    qDebug() << "to send voip message" << args;

    if (!m_client) {
        qDebug() << "ChatClient not initialized";
        return;
    }

    // 这里需要根据args构建具体的消息内容并发送
    // 实现细节需要根据具体的消息内容类型来完成

    // 示例代码框架：
    /*
    int contentType = args["content"].toObject()["type"].toInt();

    // 根据contentType创建对应的MessageContent
    std::shared_ptr<WFCLib::MessageContent> content;

    WFCLib::Conversation conversation;
    conversation.conversationType = static_cast<WFCLib::ConversationType>(args["conversation"].toObject()["type"].toInt());
    conversation.target = args["conversation"].toObject()["target"].toString().toStdString();
    conversation.line = args["conversation"].toObject()["line"].toInt();

    QStringList toUsers;
    if (args.contains("toUsers") && args["toUsers"].isArray()) {
        QJsonArray toUsersArray = args["toUsers"].toArray();
        for (const QJsonValue& value : toUsersArray) {
            toUsers.append(value.toString());
        }
    }

    std::list<std::string> toUsersList;
    for (const QString& user : toUsers) {
        toUsersList.push_back(user.toStdString());
    }

    m_client->sendMessage(conversation, content, toUsersList, 0, nullptr, 0);
    */
}

void AvEngineKitProxy::sendConferenceRequestListener(const QString& event, const QJsonObject& args)
{
    qDebug() << "to send conference request" << args;

    if (!m_client) {
        qDebug() << "ChatClient not initialized";
        return;
    }

    // 实现会议请求发送
    // m_client->sendConferenceRequest(...);
}

void AvEngineKitProxy::updateCallStartMessageContentListener(const QString& event, const QJsonObject& args)
{
    // TODO: 实现更新通话开始消息内容
    qDebug() << "update call start message" << args;
}

void AvEngineKitProxy::pickGroupMembers(const QString& event, const QJsonObject& args)
{
    qDebug() << "pickGroupMembers" << args;

    // 在Qt版本中，可以弹出群成员选择对话框
    // 这里需要实现具体的UI逻辑
}

void AvEngineKitProxy::inviteConferenceParticipant(const QString& event, const QJsonObject& args)
{
    qDebug() << "inviteConferenceParticipant" << args;

    // 实现邀请会议参与者的逻辑
}

void AvEngineKitProxy::didCallEndWithReason(const QString& event, const QJsonObject& args)
{
    qDebug() << "didCallEndWithReason" << args;

    // 通话结束，关闭通话窗口
    if (m_voipWebview) {
        m_voipWebview->close();
        setVoipWebview(nullptr);
    }
}
