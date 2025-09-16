#include "singlecallwebviewwidget.h"
#include "avenginekitproxy.h"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include "callwebinterface.h"
#include <../../wfc/client/ChatClient.h>
#include <QSettings>


 SingleCallWebViewWidget::SingleCallWebViewWidget(const WFCLib::Conversation &conversation, QList<QString> participants, bool audioOnly, QString callExtra, QWidget *parent)
    : QWidget{parent}
    , m_webView(nullptr)
{
    // 设置窗口标题和大小
    setWindowTitle("音视频通话");
    resize(240, 480);

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
    CallWebInterface *webInterface = new CallWebInterface(conversation, receivers, audioOnly, this);  // 实例化交互对象
    channel->registerObject("qtInterface", webInterface);  // 注册对象（网页中用此名称访问）
    m_webView->page()->setWebChannel(channel);  // 将channel绑定到WebView
    //m_webView->load(QUrl("qrc:/voip_web/index.html"));
    m_webView->load(QUrl("https://docs.wildfirechat.cn/webrtc/abilitytest/"));

    QSettings settings;

    QString authToken = settings.value("WFC_APPSERVER_AUTH_TOKEN").toString();
    QString clientId = QString(WFCLib::ChatClient::Instance()->getClientId().c_str());

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

void SingleCallWebViewWidget::sendMessageToWeb(const QString& message)
{
    if (m_webView) {
        // 通过JavaScript向Web页面发送消息
        QString script = QString("if(window.receiveMessageFromQt) { window.receiveMessageFromQt(%1); }").arg(message);
        m_webView->page()->runJavaScript(script);
    }
}

void SingleCallWebViewWidget::closeEvent(QCloseEvent *event)
{
    // 从AvEngineKitProxy注销
    AvEngineKitProxy::instance()->setVoipWebview(nullptr);
    QWidget::closeEvent(event);
}

