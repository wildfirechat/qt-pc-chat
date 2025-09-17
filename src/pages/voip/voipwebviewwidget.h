#ifndef VOIPWEBVIEWWIDGET_H
#define VOIPWEBVIEWWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QDebug>
#include "../../wfc/model/Conversation.h"

class QWebEngineView;
class VoipWebInterface;

class VoipWebViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VoipWebViewWidget(const QString& type, const QJsonObject& options, QWidget *parent = nullptr);

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
    VoipWebInterface *m_webInterface;
};

#endif // VOIPWEBVIEWWIDGET_H
