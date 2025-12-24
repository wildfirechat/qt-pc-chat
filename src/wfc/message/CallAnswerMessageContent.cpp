#include "CallAnswerMessageContent.h"
#include "../utility/JsonTools.h"

namespace WFCLib {
const CallAnswerMessageContentPrototype CallAnswerMessageContent::sPrototype;

MessagePayload CallAnswerMessageContent::encode() const {
    MessagePayload payload = MessageContent::encode();
    payload.content = callId;

    payload.binaryContent = audioOnly ? "1" : "0";
    JsonBuilder builder;
    builder.setValue("u", inviteMessageUid);

    payload.extra = builder.build();
    return payload;
}

void CallAnswerMessageContent::decode(const MessagePayload &payload) {
    MessageContent::decode(payload);
    callId = payload.content;
    audioOnly = (payload.binaryContent == "1");

    JsonParser parser(payload.extra);
    if (!parser.isParsed()) return;

    parser.getValue("u", inviteMessageUid);
}

MessageContent* CallAnswerMessageContent::clone() const {
    CallAnswerMessageContent *p = new CallAnswerMessageContent();
    p->extra = extra;
    p->callId = callId;
    p->audioOnly = audioOnly;
    p->inviteMessageUid = inviteMessageUid;
    return p;
}

std::string CallAnswerMessageContent::digest(const Message &message) const {
    return "";
}

const MessageContentPrototype* CallAnswerMessageContent::getPrototype() const {
    return &sPrototype;
}

}
