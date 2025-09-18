// TODO 生成 wfwebenginepage.cpp 的头文件
#ifndef WFWEBENGINEPAGE_H
#define WFWEBENGINEPAGE_H

#include <QWebEnginePage>

class WfWebEnginePage : public QWebEnginePage {
    Q_OBJECT
public:
    WfWebEnginePage(QWebEngineProfile *profile, QObject *parent = nullptr);

protected:
    void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level,
                                 const QString &message,
                                 int lineNumber,
                                 const QString &sourceID) override;
};

#endif // WFWEBENGINEPAGE_H