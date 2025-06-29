//
//  GroupSetManagerNotificationContent.cpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/31.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#include "GroupSetManagerNotificationContent.h"
#include "../utility/JsonTools.h"
#include "Message.h"
#include "../model/Conversation.h"


namespace WFCLib {

const GroupSetManagerNotificationContentPrototype GroupSetManagerNotificationContent::sPrototype;

MessagePayload GroupSetManagerNotificationContent::encode() const {
    MessagePayload payload = NotificationMessageContent::encode();
    JsonBuilder builder;
    builder.setValue("o", operatorId);
    builder.setValue("g", groupId);
    builder.setValue("n", type);
    builder.setValue("ms", memberIds);
    payload.binaryContent = builder.build();
    return payload;
}

void GroupSetManagerNotificationContent::decode(const MessagePayload & payload) {
    NotificationMessageContent::decode(payload);
    JsonParser parser(payload.binaryContent);
    if (!parser.isParsed()) {
        return;
    }
    
    parser.getValue("o", operatorId);
    parser.getValue("g", groupId);
    parser.getValue("n", type);
    parser.getValue("ms", memberIds);
}

MessageContent* GroupSetManagerNotificationContent::clone() const {
    GroupSetManagerNotificationContent *p = new GroupSetManagerNotificationContent();
    p->extra = extra;
    p->groupId = groupId;
    p->operatorId = operatorId;
    p->type = type;
    p->memberIds = memberIds;
    return p;
}

const MessageContentPrototype* GroupSetManagerNotificationContent::getPrototype() const {
    return &sPrototype;
}

std::string GroupSetManagerNotificationContent::digest(const Message &message) const {
    std::string result = getNotificationUserName(operatorId, message.conversation.target);
    
    if (type == "1") {
        result += " 设置";
    } else {
        result += " 取消";
    }
    
    for (std::list<std::string>::const_iterator it = memberIds.begin(); it != memberIds.end(); ++it) {
        result += " ";
        result += getNotificationUserName(*it, message.conversation.target);
    }
    
    if (type == "1") {
        result += " 为群管理员";
    } else {
        result += " 群管理员";
    }
    
    return result;
}

std::string GroupSetManagerNotificationContent::formatNotification(const Message &msg) const {
    return digest(msg);
}
};
