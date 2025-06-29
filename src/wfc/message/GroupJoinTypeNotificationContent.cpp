//
//  GroupJoinTypeNotificationContent.cpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/31.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#include "GroupJoinTypeNotificationContent.h"
#include "../utility/JsonTools.h"
#include "Message.h"
#include "../model/Conversation.h"


namespace WFCLib {

const GroupJoinTypeNotificationContentPrototype GroupJoinTypeNotificationContent::sPrototype;

MessagePayload GroupJoinTypeNotificationContent::encode() const {
    MessagePayload payload = NotificationMessageContent::encode();
    JsonBuilder builder;
    builder.setValue("o", operatorId);
    builder.setValue("g", groupId);
    builder.setValue("n", type);
    payload.binaryContent = builder.build();
    return payload;
}

void GroupJoinTypeNotificationContent::decode(const MessagePayload & payload) {
    NotificationMessageContent::decode(payload);
    JsonParser parser(payload.binaryContent);
    if (!parser.isParsed()) {
        return;
    }
    
    parser.getValue("o", operatorId);
    parser.getValue("g", groupId);
    parser.getValue("n", type);
}

MessageContent* GroupJoinTypeNotificationContent::clone() const {
    GroupJoinTypeNotificationContent *p = new GroupJoinTypeNotificationContent();
    p->extra = extra;
    p->groupId = groupId;
    p->operatorId = operatorId;
    p->type = type;
    return p;
}

const MessageContentPrototype* GroupJoinTypeNotificationContent::getPrototype() const {
    return &sPrototype;
}

std::string GroupJoinTypeNotificationContent::digest(const Message &message) const {
    std::string result = getNotificationUserName(operatorId, message.conversation.target);
    
    if (type == "0") {
        result += " 开启入群权限";
    } else if(type == "1") {
        result += " 允许群成员邀请入群";
    } else {
        result += " 关闭入群功能";
    }
    
    return result;
}

std::string GroupJoinTypeNotificationContent::formatNotification(const Message &msg) const {
    return digest(msg);
}
};
