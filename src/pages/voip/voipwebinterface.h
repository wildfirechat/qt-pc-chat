#ifndef VOIPWEBINTERFACE_H
#define VOIPWEBINTERFACE_H

#include <QObject>
#include "../../wfc/client/ChatClient.h"

// 1. 定义交互类（供网页调用的Qt对象）
class VoipWebInterface : public QObject
{
    Q_OBJECT  // 必须添加，支持元对象系统
public:
    explicit VoipWebInterface(QObject *parent = nullptr);

    //下面是Web调用的API
    Q_INVOKABLE QString fromWeb(const QString &message);
    Q_INVOKABLE void onReady();

signals:
    // 下面是发送给网页的请求.
    void sendToWeb(const QString &message);

};
#endif // VOIPWEBINTERFACE_H
