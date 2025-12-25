//
//  CallByeMessageContent.cpp
//  ChatClient
//
//  Created by Apple on 2025/12/25.
//  Copyright © 2025 WildFireChat. All rights reserved.
//

#include "CallByeMessageContent.h"
#include "../utility/JsonTools.h"

namespace WFCLib {
const CallByeMessageContentPrototype CallByeMessageContent::sPrototype;

MessagePayload CallByeMessageContent::encode() const
{
    MessagePayload payload = MessageContent::encode();
    payload.content = callId;

    JsonBuilder builder;
    builder.setValue("r", reason);
    builder.setValue("u", inviteMessageUid);

    payload.binaryContent = builder.build();

    return payload;
}

void CallByeMessageContent::decode(const MessagePayload & payload)
{
    MessageContent::decode(payload);
    callId = payload.content;

    JsonParser parser(payload.binaryContent);
    if (!parser.isParsed()) {
        return;
    }

    parser.getValue("r", reason);
    parser.getValue("u", inviteMessageUid);
}

MessageContent* CallByeMessageContent::clone() const {
    CallByeMessageContent *p = new CallByeMessageContent();
    p->extra = extra;
    p->callId = callId;
    p->reason = reason;
    p->inviteMessageUid = inviteMessageUid;
    return p;
}

std::string CallByeMessageContent::digest(const Message &message) const {
    return "[通话结束]";
}

const MessageContentPrototype* CallByeMessageContent::getPrototype() const {
    return &sPrototype;
}

};
