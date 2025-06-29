//
//  RecallMessageContent.cpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/31.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#include "RecallMessageContent.h"
#include "Message.h"
#include "../model/Conversation.h"

namespace WFCLib {

const RecallMessageContentPrototype RecallMessageContent::sPrototype;

MessagePayload RecallMessageContent::encode() const {
    MessagePayload payload = MessageContent::encode();
    payload.content = operatorId;
    payload.binaryContent = std::to_string(messageUid);
    return payload;
}

void RecallMessageContent::decode(const MessagePayload & payload) {
    MessageContent::decode(payload);
    operatorId = payload.content;
    messageUid = atoll(payload.binaryContent.c_str());
}

MessageContent* RecallMessageContent::clone() const {
    RecallMessageContent *p = new RecallMessageContent();
    p->extra = extra;
    p->operatorId = operatorId;
    p->messageUid = messageUid;
    return p;
}

const MessageContentPrototype* RecallMessageContent::getPrototype() const {
    return &sPrototype;
}

std::string RecallMessageContent::digest(const Message &message) const {
    std::string userName = getNotificationUserName(operatorId, message.conversation.conversationType == Group_Type?message.conversation.target:"");
    return userName + " 撤回了一条消息";
}
};
