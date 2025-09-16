#ifndef SINGLECALLWEBVIEWWIDGET_H
#define SINGLECALLWEBVIEWWIDGET_H

#include <QWidget>
#include <QDebug>
#include "../../wfc/model/Conversation.h"

class QWebEngineView;

class SingleCallWebViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SingleCallWebViewWidget(const WFCLib::Conversation &conversation, QList<QString> participants, bool audioOnly = false, QString callExtra = "", QWidget *parent = nullptr);

    /**
     * 向Web页面发送消息
     */
    void sendMessageToWeb(const QString& message);

signals:

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);

private:
    QWebEngineView *m_webView;
};

#endif // SINGLECALLWEBVIEWWIDGET_H
