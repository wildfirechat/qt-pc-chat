#ifndef AVENGINEKITPROXY_H
#define AVENGINEKITPROXY_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QWebEngineView>
#include <QJsonObject>
#include <QJsonDocument>
#include <QAudioDeviceInfo>
#include <QCameraInfo>
#include <functional>

#include "../../wfc/client/ChatClient.h"
#include "../../wfc/model/Conversation.h"
// #include "../../wfc/messages/Message.h"

class SingleCallWebViewWidget;

/**
 * Qt版本的音视频通话代理类
 * 功能等同于JavaScript版本的AvEngineKitProxy
 */
class AvEngineKitProxy : public QObject
{
    Q_OBJECT

public:
    struct QueuedEvent {
        QString event;
        QJsonObject args;
    };

    // 通话错误回调类型
    using VoipCallErrorCallback = std::function<void(int)>;
    // 通话状态回调类型
    using VoipCallStatusCallback = std::function<void(const WFCLib::Conversation&, bool)>;

    explicit AvEngineKitProxy(QObject *parent = nullptr);
    ~AvEngineKitProxy();

    static AvEngineKitProxy* instance();

    /**
     * 应用初始化时调用
     */
    void setup(WFCLib::ChatClient* client);

    /**
     * 设置音视频通话WebView
     */
    void setVoipWebview(SingleCallWebViewWidget* webview);

    /**
     * 发起音视频通话
     * @param conversation 会话
     * @param audioOnly 是否是音频通话
     * @param participants 参与者用户id列表
     * @param callExtra 通话附加信息
     */
    void startCall(const WFCLib::Conversation& conversation, bool audioOnly,
                   const QStringList& participants, const QString& callExtra = "");

    /**
     * 开始会议
     */
    void startConference(const QString& callId, bool audioOnly, const QString& pin,
                        const QString& host, const QString& title, const QString& desc,
                        bool audience, bool advance, bool record = false,
                        const QJsonObject& extra = QJsonObject(),
                        const QJsonObject& callExtra = QJsonObject(),
                        bool muteAudio = false, bool muteVideo = false);

    /**
     * 加入会议
     */
    void joinConference(const QString& callId, bool audioOnly, const QString& pin,
                       const QString& host, const QString& title, const QString& desc,
                       bool audience, bool advance, bool muteAudio, bool muteVideo,
                       const QJsonObject& extra = QJsonObject(),
                       const QJsonObject& callExtra = QJsonObject());

    /**
     * 设置通话错误回调
     */
    void setVoipCallErrorCallback(VoipCallErrorCallback callback);

    /**
     * 设置通话状态回调
     */
    void setVoipCallStatusCallback(VoipCallStatusCallback callback);

    /**
     * 获取当前会话
     */
    WFCLib::Conversation getCurrentConversation() const { return m_conversation; }

    /**
     * 获取当前通话ID
     */
    QString getCurrentCallId() const { return m_callId; }

    /**
     * 是否正在通话中
     */
    bool isInCall() const { return m_voipWebview != nullptr; }

    // WebView事件监听器
    void voipWebviewEventListener(const QString& event, const QJsonObject& args);

public slots:
    /**
     * 向通话WebView发送事件
     */
    void emitToVoip(const QString& event, const QJsonObject& args);

    /**
     * 向主窗口发送事件（兼容接口）
     */
    void emitToMain(const QString& event, const QJsonObject& args);

    /**
     * 接收消息处理
     */
    void onReceiveMessages(const std::list<WFCLib::Message>& messages, bool hasMore);

protected:
    // 移除ReceiveMessageCallback接口实现

private slots:
    void onConferenceEvent(const QJsonObject& event);
    void checkDevices();

private:
    void processQueuedEvents();
    void showCallUI(const WFCLib::Conversation& conversation, bool isConference, const QJsonObject& options);

    // 消息处理方法
    void handleVoipMessage(const WFCLib::Message& msg);
    bool isVoipMessage(int messageType);
    bool isExpiredMessage(const WFCLib::Message& msg);

    // 事件处理方法
    void sendVoipListener(const QString& event, const QJsonObject& args);
    void sendConferenceRequestListener(const QString& event, const QJsonObject& args);
    void updateCallStartMessageContentListener(const QString& event, const QJsonObject& args);
    void pickGroupMembers(const QString& event, const QJsonObject& args);
    void inviteConferenceParticipant(const QString& event, const QJsonObject& args);
    void didCallEndWithReason(const QString& event, const QJsonObject& args);

private:
    static AvEngineKitProxy* s_instance;

    WFCLib::ChatClient* m_client;
    QQueue<QueuedEvent> m_queueEvents;

    // 通话状态
    bool m_conference;
    WFCLib::Conversation m_conversation;
    QString m_callId;
    int64_t m_inviteMessageUid;
    QStringList m_participants;

    // 设备支持状态
    bool m_isSupportVoip;
    bool m_hasMicrophone;
    bool m_hasSpeaker;
    bool m_hasWebcam;

    // UI相关
    SingleCallWebViewWidget* m_voipWebview;
    bool m_debug;

    // 回调函数
    VoipCallErrorCallback m_onVoipCallErrorCallback;
    VoipCallStatusCallback m_onVoipCallStatusCallback;

    QTimer* m_deviceCheckTimer;
};

#endif // AVENGINEKITPROXY_H
