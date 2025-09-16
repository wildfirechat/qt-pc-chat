# Qt版本AvEngineKitProxy使用说明

## 概述

Qt版本的AvEngineKitProxy是JavaScript版本avenginekitproxy.js的C++实现，提供了完全一致的音视频通话功能。它作为音视频通话的核心代理组件，负责管理通话状态、处理消息传递和协调UI交互。

## 主要功能

### 1. 通话管理
- **发起通话**: 支持音频通话和视频通话
- **接听通话**: 处理来电邀请
- **挂断通话**: 结束正在进行的通话
- **通话状态管理**: 跟踪当前通话状态

### 2. 会议功能
- **创建会议**: 发起多人会议
- **加入会议**: 参与现有会议
- **会议管理**: 管理会议参与者和设置

### 3. 消息处理
- **VoIP消息**: 处理通话相关的信令消息
- **会议消息**: 处理会议相关的消息
- **消息队列**: 管理待处理的事件队列

### 4. 设备检测
- **麦克风检测**: 检查音频输入设备
- **扬声器检测**: 检查音频输出设备
- **摄像头检测**: 检查视频输入设备

## 使用方法

### 初始化

```cpp
#include "avenginekitproxy.h"

// 在应用启动时初始化
AvEngineKitProxy::instance()->setup(WFCLib::ChatClient::Instance());

// 设置通话错误回调
AvEngineKitProxy::instance()->setVoipCallErrorCallback([](int errorCode) {
    switch(errorCode) {
        case -1:
            qDebug() << "已有通话正在进行中";
            break;
        case -2:
            qDebug() << "设备不支持音视频通话";
            break;
    }
});

// 设置通话状态回调
AvEngineKitProxy::instance()->setVoipCallStatusCallback([](const WFCLib::Conversation& conv, bool ongoing) {
    qDebug() << "通话状态变更:" << ongoing;
});
```

### 发起通话

```cpp
// 发起单人音频通话
WFCLib::Conversation conversation;
conversation.conversationType = WFCLib::Single_Type;
conversation.target = "targetUserId";
conversation.line = 0;

QStringList participants = {"targetUserId"};
AvEngineKitProxy::instance()->startCall(conversation, true, participants); // true = 音频通话

// 发起视频通话
AvEngineKitProxy::instance()->startCall(conversation, false, participants); // false = 视频通话

// 发起群组通话
conversation.conversationType = WFCLib::Group_Type;
conversation.target = "groupId";
QStringList groupParticipants = {"user1", "user2", "user3"};
AvEngineKitProxy::instance()->startCall(conversation, false, groupParticipants);
```

### 会议功能

```cpp
// 创建会议
QString callId = "meeting_" + QString::number(QDateTime::currentMSecsSinceEpoch());
AvEngineKitProxy::instance()->startConference(
    callId,           // 会议ID
    false,            // 是否仅音频
    "123456",         // PIN码
    "hostUserId",     // 主持人
    "会议标题",        // 标题
    "会议描述",        // 描述
    false,            // 是否为观众模式
    false,            // 是否为高级会议
    false,            // 是否录制
    QJsonObject(),    // 额外信息
    QJsonObject(),    // 通话额外信息
    false,            // 静音加入
    false             // 关闭摄像头加入
);

// 加入会议
AvEngineKitProxy::instance()->joinConference(
    "existingMeetingId", // 现有会议ID
    false,               // 是否仅音频
    "123456",            // PIN码
    "hostUserId",        // 主持人
    "会议标题",           // 标题
    "会议描述",           // 描述
    false,               // 是否为观众
    false,               // 是否为高级会议
    false,               // 静音加入
    false                // 关闭摄像头加入
);
```

### 检查通话状态

```cpp
// 检查是否正在通话中
if (AvEngineKitProxy::instance()->isInCall()) {
    qDebug() << "当前正在通话中";
}

// 获取当前会话
WFCLib::Conversation currentConv = AvEngineKitProxy::instance()->getCurrentConversation();

// 获取当前通话ID
QString callId = AvEngineKitProxy::instance()->getCurrentCallId();
```

## 架构设计

### 核心组件

1. **AvEngineKitProxy**: 主要代理类，管理所有通话相关逻辑
2. **SingleCallWebViewWidget**: 通话UI窗口，内嵌WebView显示通话界面
3. **CallWebInterface**: Qt与Web页面的通信接口

### 通信流程

```
ChatDetailWidget (发起通话)
    ↓
AvEngineKitProxy (处理通话逻辑)
    ↓
SingleCallWebViewWidget (显示通话UI)
    ↓
WebView + CallWebInterface (与Web通话页面交互)
```

### 消息处理流程

```
WFC消息接收 → AvEngineKitProxy::onReceiveMessage → 过滤VoIP消息 → 发送到WebView
```

## 与JavaScript版本的对应关系

| JavaScript方法 | Qt方法 | 说明 |
|---------------|--------|------|
| `setup(wfc)` | `setup(ChatClient*)` | 初始化代理 |
| `startCall()` | `startCall()` | 发起通话 |
| `startConference()` | `startConference()` | 开始会议 |
| `joinConference()` | `joinConference()` | 加入会议 |
| `emitToVoip()` | `emitToVoip()` | 向WebView发送事件 |
| `onReceiveMessage` | `onReceiveMessage()` | 接收消息回调 |

## 注意事项

1. **设备权限**: 确保应用已获得麦克风和摄像头权限
2. **网络连接**: 通话功能需要稳定的网络连接
3. **WebRTC支持**: 确保Qt WebEngine支持WebRTC功能
4. **消息过滤**: 只有VoIP相关的消息会被处理
5. **状态同步**: 通话状态会在多个组件间同步

## 错误处理

- **-1**: 已有通话正在进行中
- **-2**: 设备不支持音视频通话
- 消息过期: 超过90秒的消息会被忽略
- 设备检测失败: 会在回调中通知

## 扩展功能

可以通过继承或扩展以下接口来添加更多功能：

1. 自定义消息处理
2. 额外的设备检测
3. 通话录制功能
4. 自定义UI主题

这个Qt版本的AvEngineKitProxy提供了与JavaScript版本完全一致的API和功能，确保了跨平台的兼容性和一致的用户体验。