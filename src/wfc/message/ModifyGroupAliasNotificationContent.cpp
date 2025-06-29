//
//  ModifyGroupAliasNotificationContent.cpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/31.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#include "ModifyGroupAliasNotificationContent.h"
#include "../utility/JsonTools.h"
#include "Message.h"
#include "../model/Conversation.h"


namespace WFCLib {

const ModifyGroupAliasNotificationContentPrototype ModifyGroupAliasNotificationContent::sPrototype;

MessagePayload ModifyGroupAliasNotificationContent::encode() const {
    MessagePayload payload = NotificationMessageContent::encode();
    JsonBuilder builder;
    builder.setValue("o", operatorId);
    builder.setValue("g", groupId);
    builder.setValue("n", alias);
    payload.binaryContent = builder.build();
    return payload;
}

void ModifyGroupAliasNotificationContent::decode(const MessagePayload & payload) {
    NotificationMessageContent::decode(payload);
    JsonParser parser(payload.binaryContent);
    if (!parser.isParsed()) {
        return;
    }
    
    parser.getValue("o", operatorId);
    parser.getValue("g", groupId);
    parser.getValue("n", alias);
}

MessageContent* ModifyGroupAliasNotificationContent::clone() const {
    ModifyGroupAliasNotificationContent *p = new ModifyGroupAliasNotificationContent();
    p->extra = extra;
    p->groupId = groupId;
    p->operatorId = operatorId;
    p->alias = alias;
    return p;
}

const MessageContentPrototype* ModifyGroupAliasNotificationContent::getPrototype() const {
    return &sPrototype;
}

std::string ModifyGroupAliasNotificationContent::digest(const Message &message) const {
    std::string result = getNotificationUserName(operatorId, message.conversation.target);
    
    result += " 更改了群昵称";
    
    return result;
}

std::string ModifyGroupAliasNotificationContent::formatNotification(const Message &msg) const {
    return digest(msg);
}
};
