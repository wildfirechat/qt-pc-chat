//
//  GroupPrivateChatNotificationContent.cpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/31.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#include "GroupPrivateChatNotificationContent.h"
#include "../utility/JsonTools.h"
#include "Message.h"
#include "../model/Conversation.h"


namespace WFCLib {

const GroupPrivateChatNotificationContentPrototype GroupPrivateChatNotificationContent::sPrototype;

MessagePayload GroupPrivateChatNotificationContent::encode() const {
    MessagePayload payload = NotificationMessageContent::encode();
    JsonBuilder builder;
    builder.setValue("o", operatorId);
    builder.setValue("g", groupId);
    builder.setValue("n", type);
    payload.binaryContent = builder.build();
    return payload;
}

void GroupPrivateChatNotificationContent::decode(const MessagePayload & payload) {
    NotificationMessageContent::decode(payload);
    JsonParser parser(payload.binaryContent);
    if (!parser.isParsed()) {
        return;
    }
    
    parser.getValue("o", operatorId);
    parser.getValue("g", groupId);
    parser.getValue("n", type);
}

MessageContent* GroupPrivateChatNotificationContent::clone() const {
    GroupPrivateChatNotificationContent *p = new GroupPrivateChatNotificationContent();
    p->extra = extra;
    p->groupId = groupId;
    p->operatorId = operatorId;
    p->type = type;
    return p;
}

const MessageContentPrototype* GroupPrivateChatNotificationContent::getPrototype() const {
    return &sPrototype;
}

std::string GroupPrivateChatNotificationContent::digest(const Message &message) const {
    std::string result = getNotificationUserName(operatorId, message.conversation.target);
    
    if (type == "1") {
        result += " 关闭";
    } else {
        result += " 开启";
    }
    result += " 成员私聊功能";
    
    return result;
}

std::string GroupPrivateChatNotificationContent::formatNotification(const Message &msg) const {
    return digest(msg);
}
};
