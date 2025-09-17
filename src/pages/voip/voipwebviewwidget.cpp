#include "voipwebviewwidget.h"
#include "avenginekitproxy.h"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QCloseEvent>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include "voipwebinterface.h"
#include <../../wfc/client/ChatClient.h>
#include <QSettings>
#include "../../config.h"


 VoipWebViewWidget::VoipWebViewWidget(const QString& type, const QJsonObject& options, QWidget *parent)
    : QWidget{parent}
    , m_webView(nullptr)
    , m_webInterface(nullptr)
{
    // 设置窗口标题和大小
    setWindowTitle("音视频通话");
    resize(480, 640);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建并配置QWebEngineView
    m_webView = new QWebEngineView(this);

    // 将webView添加到布局
    layout->addWidget(m_webView);


    // 创建按钮（用于Qt主动调用网页）
    QPushButton *btn = new QPushButton("向网页发送消息", this);
    layout->addWidget(btn);


    // 3. 配置WebChannel
    QWebChannel *channel = new QWebChannel(this);
    QList<QString> receivers;
    m_webInterface = new VoipWebInterface(this);  // 实例化交互对象并保存引用
    channel->registerObject("qtInterface", m_webInterface);  // 注册对象（网页中用此名称访问）
    m_webView->page()->setWebChannel(channel);  // 将channel绑定到WebView

    QSettings settings;

    QString voipBaseUrl = "qrc:/voip_web/index.html";

    QString authToken = settings.value("WFC_APPSERVER_AUTH_TOKEN").toString();
    QString clientId = QString(WFCLib::ChatClient::Instance()->getClientId().c_str());

    // 实现类似 JS 版本的 URL 拼接逻辑
    // 获取短链接信息 (类似 wfc.getShortLinkInfo())
    QString server = QString(WFCLib::ChatClient::Instance()->getHost().c_str());
    QString userId = QString(WFCLib::ChatClient::Instance()->getCurrentUserId().c_str());
    QString token = settings.value("WFC_IM_TOKEN").toString(); // 从设置中获取保存的 token

    // 处理 token，替换特殊字符 (类似 JS 版本)
    token = token.replace('+', '.').replace('/', '_').replace('=', '-');

    // 拼接 voip web URL
    QString voipWebUrl = voipBaseUrl;


    voipWebUrl += "?type=" + type;

    // app server info
    voipWebUrl += "&appServer=" + QUrl::toPercentEncoding(APP_SERVER_ADDRESS);
    voipWebUrl += "&authToken=" + authToken;

    // im server info
    voipWebUrl += "&server=" + QUrl::toPercentEncoding(server);
    voipWebUrl += "&userId=" + userId;
    voipWebUrl += "&token=" + token;
    voipWebUrl += "&clientId=" + clientId;

    // options (将整个 options 对象转为 JSON 字符串)
    QJsonDocument optionsDoc(options);
    QString optionsStr = QString(optionsDoc.toJson(QJsonDocument::Compact));
    voipWebUrl += "&options=" + QUrl::toPercentEncoding(optionsStr);

    // enable voip debug
    voipWebUrl += "&debug=false";

    // 加载拼接好的 URL
    qDebug() << "Loading voip web URL:" << voipWebUrl;
    m_webView->load(QUrl(voipWebUrl));



    // m_webView->load(QUrl("qrc:/voip_web/voip-conf.html"));
    //m_webView->load(QUrl("https://docs.wildfirechat.cn/webrtc/abilitytest/"));

    // 注册到AvEngineKitProxy
    AvEngineKitProxy::instance()->setVoipWebview(this);

    // // 4. Qt调用网页：点击按钮时执行网页的JavaScript函数
    // connect(btn, &QPushButton::clicked, this, [webView, webInterface]() {
    //     QString msg = "Hello from Qt!";
    //     // 发送通知
    //     webInterface->sendToWeb(msg);
    // });
    // connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::receiveMessages, this, &ChatDetailWidget::onReceiveMessages);

}

void VoipWebViewWidget::sendMessageToWeb(const QString& message)
{
    if (m_webInterface) {
        // 使用VoipWebInterface的sendToWeb方法向Web页面发送消息
        m_webInterface->sendToWeb(message);
    }
}

void VoipWebViewWidget::closeEvent(QCloseEvent *event)
{
    // 从AvEngineKitProxy注销
    AvEngineKitProxy::instance()->setVoipWebview(nullptr);
    QWidget::closeEvent(event);
}

