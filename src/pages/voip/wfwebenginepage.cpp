// 1. 创建自定义 WebEnginePage 类
// wfwebenginepage.cpp
#include "wfwebenginepage.h"
#include <QDebug>

WfWebEnginePage::WfWebEnginePage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent) {
    // 初始化代码...
}

void WfWebEnginePage::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level,
                                             const QString &message,
                                             int lineNumber,
                                             const QString &sourceID) {
    // 实现代码，捕获JS控制台消息
    qDebug() << "WFC JS:" << message << "at" << sourceID << ":" << lineNumber;

    // 可以增加特定消息的处理逻辑

    // 调用基类实现（可选）
    QWebEnginePage::javaScriptConsoleMessage(level, message, lineNumber, sourceID);
}

// 其他方法的实现...