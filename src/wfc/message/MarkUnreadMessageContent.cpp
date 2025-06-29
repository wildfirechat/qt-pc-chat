#include "MarkUnreadMessageContent.h"
#include "../utility/JsonTools.h"


namespace WFCLib {

const MarkUnreadMessageContentPrototype MarkUnreadMessageContent::sPrototype;

MarkUnreadMessageContent::MarkUnreadMessageContent() {}

MessagePayload MarkUnreadMessageContent::encode() const
{
    MessagePayload payload = MessageContent::encode();
    JsonBuilder builder;
    builder.setValue("u", messageUid);
    builder.setValue("t", timestamp);

    payload.binaryContent = builder.build();

    return payload;
}

void MarkUnreadMessageContent::decode(const MessagePayload &payload)
{
    MessageContent::decode(payload);

    JsonParser parser(payload.binaryContent);
    if (!parser.isParsed()) {
        return;
    }

    parser.getValue("u", messageUid);
    parser.getValue("t", timestamp);
}

MessageContent *MarkUnreadMessageContent::clone() const
{
    MarkUnreadMessageContent *c = new MarkUnreadMessageContent();
    c->extra = extra;
    c->messageUid = messageUid;
    c->timestamp = timestamp;
    return c;
}

std::string MarkUnreadMessageContent::digest(const Message &message) const
{
    return "未知消息";
}

const MessageContentPrototype *MarkUnreadMessageContent::getPrototype() const
{
    return &sPrototype;
}

};
