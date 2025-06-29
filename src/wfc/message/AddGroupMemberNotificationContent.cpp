//
//  AddGroupMemberNotificationContent.cpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/31.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#include "AddGroupMemberNotificationContent.h"
#include "../utility/JsonTools.h"
#include "Message.h"
#include "../model/Conversation.h"

namespace WFCLib {
const AddGroupMemberNotificationContentPrototype AddGroupMemberNotificationContent::sPrototype;

AddGroupMemberNotificationContent::AddGroupMemberNotificationContent()
{
}

MessagePayload AddGroupMemberNotificationContent::encode() const
{
    MessagePayload payload = NotificationMessageContent::encode();
    
    JsonBuilder builder;
    builder.setValue("g", groupId);
    builder.setValue("o", operatorId);
    builder.setValue("ms", invitees);
    
    payload.binaryContent = builder.build();

    return payload;
}

MessageContent* AddGroupMemberNotificationContent::clone() const {
    AddGroupMemberNotificationContent *p = new AddGroupMemberNotificationContent();
    p->extra = extra;
    p->groupId = groupId;
    p->operatorId = operatorId;
    p->invitees = invitees;
    return p;
}

const MessageContentPrototype* AddGroupMemberNotificationContent::getPrototype() const {
    return &sPrototype;
}

void AddGroupMemberNotificationContent::decode(const MessagePayload & payload)
{
    NotificationMessageContent::decode(payload);
    
    JsonParser parser(payload.binaryContent);
    if (!parser.isParsed()) {
        return;
    }

    parser.getValue("o", operatorId);
    parser.getValue("g", groupId);
    parser.getValue("ms", invitees);
}

std::string AddGroupMemberNotificationContent::digest(const Message &message) const {
    std::string result = getNotificationUserName(operatorId, message.conversation.target);
    
    if (invitees.size() == 1 && invitees.front() == operatorId) {
        return result + " 加入群组";
    }
    
    result += " 邀请 ";


    for (std::list<std::string>::const_iterator it = invitees.begin(); it != invitees.end(); ++it) {
        const std::string &userId = *it;
        result += " ";
        result += getNotificationUserName(userId, message.conversation.target);
    }
    result += " 加入群组";
    
    return result;
}

std::string AddGroupMemberNotificationContent::formatNotification(const Message &msg) const {
    return digest(msg);
}

AddGroupMemberNotificationContent::~AddGroupMemberNotificationContent()
{
}

MessageContentType AddGroupMemberNotificationContentPrototype::getType() const
{
    return MESSAGE_CONTENT_TYPE_ADD_GROUP_MEMBER;
}

MessageContentPersistFlag AddGroupMemberNotificationContentPrototype::getFlag() const
{
    return PersistFlag_PERSIST;
}

MessageContent * AddGroupMemberNotificationContentPrototype::productEmptyContent() const
{
    return new AddGroupMemberNotificationContent();
}
};
