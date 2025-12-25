#include "voipwebviewwidget.h"
#include "avenginekitproxy.h"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEnginePage>
#include "./wfwebenginepage.h"
#include <QWebChannel>
#include <QCloseEvent>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include "voipwebinterface.h"
#include <../../wfc/client/ChatClient.h>
#include <QSettings>
#include "../../config.h"

VoipWebViewWidget::VoipWebViewWidget(const QString &type, const QJsonObject &options, QWidget *parent)
    : QWidget{parent}, m_webView(nullptr), m_webInterface(nullptr)
{
    // 设置窗口标题和大小
    setWindowTitle("音视频通话");
    resize(480, 640);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // WfWebEnginePage *profile = WfWebEnginePage::defaultProfile();
    // // profile->setHttpUserAgent("自定义UA"); // 模拟更真实浏览器
    // profile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    // profile->setSpellCheckEnabled(false);
    // profile->settings()->setAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly, false);
    // profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36");

    // 创建profile并设置UA
    QWebEngineProfile* profile = QWebEngineProfile::defaultProfile();
    profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36");

    // Enable WebRTC and other necessary settings
    profile->settings()->setAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly, false);
    profile->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    profile->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    profile->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);

    // 创建并配置QWebEngineView
    m_webView = new QWebEngineView(this);
    // 创建WfWebEnginePage并设置给WebView
    WfWebEnginePage* page = new WfWebEnginePage(profile, m_webView);
    m_webView->setPage(page);

    connect(m_webView, &QWebEngineView::loadFinished, [=](bool ok)
            {
    if (!ok) {
        qDebug() << "页面加载失败:" << m_webView->url();
        // 这里无法获得详细错误类型，只知道 ok 为 false
    } });
    connect(m_webView, &QWebEngineView::loadFinished, [=](bool ok)
            {
    if (!ok) {
        qDebug() << "页面加载失败";
        m_webView->page()->runJavaScript(
            "console.log('Debug info:', { "
            "  wfc: typeof window.wfc, "
            "  navigator: navigator.userAgent, "
            "  features: { "
            "    webrtc: !!window.RTCPeerConnection, "
            "    websocket: !!window.WebSocket "
            "  }"
            "});"
        );
    } });

    connect(m_webView->page(), &QWebEnginePage::featurePermissionRequested,
        [=](const QUrl &securityOrigin, QWebEnginePage::Feature feature) {
        if (feature == QWebEnginePage::MediaAudioCapture ||
            feature == QWebEnginePage::MediaAudioVideoCapture ||
            feature == QWebEnginePage::MediaVideoCapture) {
            // You might want to show a native dialog here to confirm with the user,
            // but for basic functionality, you can just accept.
            m_webView->page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
        }
    });


    // 正确的 Qt 5.15 连接方式
    // connect(m_webView->page(), &WfQWebEnginePage::javaScriptConsoleMessage,
    //         [](JavaScriptConsoleMessageLevel level, const QString &message,
    //            int lineNumber, const QString &sourceID)
    //         {
    //             qDebug() << "JS Console:" << message << "at" << sourceID << ":" << lineNumber;
    //         });
    // 创建 DevTools 页面
    // QWebEngineView *devToolsView = new QWebEngineView;
    // m_webView->page()->setDevToolsPage(devToolsView->page());
    // devToolsView->show(); // 或嵌入到你的布局

    // 将webView添加到布局
    layout->addWidget(m_webView);

    // 3. 配置WebChannel
    QWebChannel *channel = new QWebChannel(this);
    QList<QString> receivers;
    m_webInterface = new VoipWebInterface(this);            // 实例化交互对象并保存引用
    channel->registerObject("qtInterface", m_webInterface); // 注册对象（网页中用此名称访问）
    m_webView->page()->setWebChannel(channel);              // 将channel绑定到WebView

    QSettings settings;

    // QString voipBaseUrl = "https://custom.wildfirechat.cn/wfim_deploy_cookbook/pages/test.html";
    // QString voipBaseUrl = "qrc:/voip_web/index.html";
    QString voipBaseUrl = "http://localhost:8082";
    // QString voipBaseUrl = "https://static.wildfirechat.cn/voip-conf-20250915-2.html";

    QString authToken = settings.value("WFC_APPSERVER_AUTH_TOKEN").toString();
    QString clientId = QString(WFCLib::ChatClient::Instance()->getClientId().c_str());

    // 实现类似 JS 版本的 URL 拼接逻辑
    // 获取短链接信息 (类似 wfc.getShortLinkInfo())
    QString server = QString(WFCLib::ChatClient::Instance()->getHost().c_str());
    QString userId = QString(WFCLib::ChatClient::Instance()->getCurrentUserId().c_str());

    QSettings im_settings("WfcData", "WFC");
    QString token = im_settings.value("wfc/token", "").toString();

    qDebug() << "token ....voip web URL:" << token;

    // 处理 token，替换特殊字符 (类似 JS 版本)
    token = token.replace('+', '.').replace('/', '_').replace('=', '-');

    // 拼接 voip web URL
    QString voipWebUrl = voipBaseUrl;

    voipWebUrl += "?type=" + type;

    // app server info
    voipWebUrl += "&appServer=" + QUrl::toPercentEncoding(APP_SERVER_ADDRESS);
    voipWebUrl += "&authToken=" + authToken;

    // im server info
    // voipWebUrl += "&server=http://" + QUrl::toPercentEncoding(server);
    voipWebUrl += "&server=https://wildfirechat.net"; ;
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
    // m_webView->load(QUrl(voipBaseUrl));

    //m_webView->load(QUrl("qrc:/voip_web/index.html"));
    // m_webView->load(QUrl("https://docs.wildfirechat.cn/webrtc/abilitytest/"));
    // m_webView->load(QUrl("http://192.168.2.101:8080/"));
    // m_webView->load(QUrl("https://web.wildfirechat.cn/"));
    // m_webView->load(QUrl("https://static.wildfirechat.cn/voip-conf-20250915-2.html"));

    // 注册到AvEngineKitProxy
    AvEngineKitProxy::instance()->setVoipWebview(this);

    // Enable developer tools for debugging
    QWebEngineView *devToolsView = new QWebEngineView;
    page->setDevToolsPage(devToolsView->page());
    devToolsView->show();

    connect(m_webView->page(), &QWebEnginePage::windowCloseRequested, this, &VoipWebViewWidget::close);
}

void VoipWebViewWidget::sendMessageToWeb(const QString &message)
{
    if (m_webInterface)
    {
        // 使用VoipWebInterface的sendToWeb方法向Web页面发送消息
        qDebug() << "Sending message to web:" << message;
        m_webInterface->sendToWeb(message);
    }
}

void VoipWebViewWidget::closeEvent(QCloseEvent *event)
{
    // 从AvEngineKitProxy注销
    AvEngineKitProxy::instance()->setVoipWebview(nullptr);
    QWidget::closeEvent(event);
}
