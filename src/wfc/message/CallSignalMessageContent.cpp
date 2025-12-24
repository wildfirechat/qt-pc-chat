#include "CallSignalMessageContent.h"
#include "../utility/JsonTools.h"

namespace WFCLib {
const CallSignalMessageContentPrototype CallSignalMessageContent::sPrototype;

MessagePayload CallSignalMessageContent::encode() const {
    MessagePayload payload = MessageContent::encode();
    payload.content = callId;
    payload.binaryContent = signalPayload;
    return payload;
}

void CallSignalMessageContent::decode(const MessagePayload &payload) {
    MessageContent::decode(payload);
    callId = payload.content;
    signalPayload = payload.binaryContent;
}

MessageContent* CallSignalMessageContent::clone() const {
    CallSignalMessageContent *p = new CallSignalMessageContent();
    p->extra = extra;
    p->callId = callId;
    p->signalPayload = signalPayload;
    return p;
}

std::string CallSignalMessageContent::digest(const Message &message) const {
    return "";
}

const MessageContentPrototype* CallSignalMessageContent::getPrototype() const {
    return &sPrototype;
}

}
