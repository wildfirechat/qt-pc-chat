//
//  Message.cpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/29.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#include "Message.h"
#include "../utility/JsonTools.h"
#include "../client/ChatClient.h"

namespace WFCLib {
void Message::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

//            Conversation conversation;
    getValue(value, "conversation", conversation);
//            std::string from;
    getValue(value, "from", from);
//            TMessageContent content;
    MessagePayload payload;
    getValue(value, "content", payload);
    content = ChatClient::Instance()->getContent(payload);
    
//            long messageId;
    getValue(value, "messageId", messageId);
//            int direction;
    int dir;
    getValue(value, "direction", dir);
    direction = (MessageDirection)dir;
    
//            MessageStatus status;
    int iStatus = 0;
    getValue(value, "status", iStatus);
    status = (MessageStatus)iStatus;

//            int64_t messageUid;
    std::string strUid;
    getValue(value, "messageUid", strUid);
    messageUid = atoll(strUid.c_str());
    
//            int64_t timestamp;
    getValue(value, "timestamp", timestamp);
//            std::string to;
    getValue(value, "to", to);
}

void Message::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("conversation");
    conversation.Serialize(&writer);

//            std::string from;
    writer.String("from");
    writer.String(from);

//            TMessageContent content;
    writer.String("content");
    MessagePayload payload = content->encode();
    payload.Serialize(&writer);

//            long messageId;
    writer.String("messageId");
    writer.Int64(messageId);

//            int direction;
    writer.String("direction");
    writer.Int(direction);

//            MessageStatus status;
    writer.String("status");
    writer.Int((int)status);

//            int64_t messageUid;
    writer.String("messageUid");
    writer.String(longlongToStr(messageUid));

//            int64_t timestamp;
    writer.String("timestamp");
    writer.Int64(timestamp);

//            std::string to;
    writer.String("to");
    writer.StartArray();
    for (std::list<std::string>::const_iterator it = to.begin(); it != to.end(); ++it) {
        writer.String(*it);
    }
    writer.EndArray();

    writer.EndObject();
}

Message::Message(const Message &m) {
    conversation = m.conversation;
    from = m.from;
    if(m.content) {
        content = m.content->clone();
    } else {
        content = nullptr;
    }
    messageId = m.messageId;
    direction = m.direction;
    status = m.status;
    messageUid = m.messageUid;
    timestamp = m.timestamp;
    to = m.to;
}

Message &Message::operator=(const Message &m)
{
    conversation = m.conversation;
    from = m.from;
    if(content != nullptr) {
        delete content;
    }
    if(m.content) {
        content = m.content->clone();
    } else {
        content = nullptr;
    }
    messageId = m.messageId;
    direction = m.direction;
    status = m.status;
    messageUid = m.messageUid;
    timestamp = m.timestamp;
    to = m.to;
    return *this;
}

Message::Message(Message &&other) noexcept {
    conversation = other.conversation;
    from = other.from;
    content = other.content;
    other.content = nullptr; // 转移资源所有权
    messageId = other.messageId;
    direction = other.direction;
    status = other.status;
    messageUid = other.messageUid;
    timestamp = other.timestamp;
    to = other.to;
}

Message &Message::operator=(Message &&other) noexcept {
    if (this != &other) {
        if(content != nullptr) {
            delete content;
        }
        content = other.content;
        other.content = nullptr; // 转移资源所有权
        conversation = other.conversation;
        from = other.from;
        messageId = other.messageId;
        direction = other.direction;
        status = other.status;
        messageUid = other.messageUid;
        timestamp = other.timestamp;
        to = other.to;
    }
    return *this;
}

};
