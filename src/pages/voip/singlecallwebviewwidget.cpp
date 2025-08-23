#include "singlecallwebviewwidget.h"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include "callwebinterface.h"


SingleCallWebViewWidget::SingleCallWebViewWidget(QWidget *parent)
    : QWidget{parent}
{
    // 设置窗口标题和大小
    setWindowTitle("百度搜索");
    resize(240, 480);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建并配置QWebEngineView
    QWebEngineView *webView = new QWebEngineView(this);

    // 将webView添加到布局
    layout->addWidget(webView);


    // 创建按钮（用于Qt主动调用网页）
    QPushButton *btn = new QPushButton("向网页发送消息", this);
    layout->addWidget(btn);


    // 3. 配置WebChannel
    QWebChannel *channel = new QWebChannel(this);
    WFCLib::Conversation conversation;
    QList<QString> receivers;
    CallWebInterface *webInterface = new CallWebInterface(conversation, receivers, false, this);  // 实例化交互对象
    channel->registerObject("qtInterface", webInterface);  // 注册对象（网页中用此名称访问）
    webView->page()->setWebChannel(channel);  // 将channel绑定到WebView
    webView->load(QUrl("qrc:/voip_web/index.html"));

    // 4. Qt调用网页：点击按钮时执行网页的JavaScript函数
    connect(btn, &QPushButton::clicked, this, [webView, webInterface]() {
        QString msg = "Hello from Qt!";
        // 发送通知
        webInterface->sendToWeb(msg);
    });

}

void SingleCallWebViewWidget::closeEvent(QCloseEvent *event)
{

}
