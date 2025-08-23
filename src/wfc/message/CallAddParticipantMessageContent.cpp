#include "CallAddParticipantMessageContent.h"
#include "../utility/JsonTools.h"
#include "Message.h"
#include "../model/Conversation.h"

namespace WFCLib {
const CallAddParticipantMessageContentPrototype CallAddParticipantMessageContent::sPrototype;

CallAddParticipantMessageContent::CallAddParticipantMessageContent()
{
}

MessagePayload CallAddParticipantMessageContent::encode() const
{
    MessagePayload payload = NotificationMessageContent::encode();
    payload.content = callId;

    JsonBuilder builder;
    builder.setValue("initiator", initiator);
    if(!pin.empty()) builder.setValue("pin", pin);
    if(!participants.empty()) builder.setValue("participants", participants);
    if(audioOnly) builder.setValue("audioOnly", 1);
    if(!existParticipants.empty()) builder.setValue("existParticipants", existParticipants);
    if(autoAnswer) builder.setValue("autoAnswer", 1);
    if(!clientId.empty()) builder.setValue("clientId", clientId);
    payload.binaryContent = builder.build();

    JsonBuilder builde2r;
    if(!participants.empty()) builde2r.setValue("participants", participants);
    if(!existParticipants.empty()) {
        std::list<std::string> ps;
        for(auto p : existParticipants) {
            ps.push_back(p.userId);
        }
        builde2r.setValue("existParticipants", ps);
    }
    payload.pushData = builde2r.build();

    return payload;
}

MessageContent* CallAddParticipantMessageContent::clone() const {
    CallAddParticipantMessageContent *p = new CallAddParticipantMessageContent();
    *p = *this;
    return p;
}

const MessageContentPrototype* CallAddParticipantMessageContent::getPrototype() const {
    return &sPrototype;
}

void CallAddParticipantMessageContent::decode(const MessagePayload & payload)
{
    NotificationMessageContent::decode(payload);
    callId = payload.content;

    JsonParser parser(payload.binaryContent);
    if (!parser.isParsed()) {
        return;
    }

    parser.getValue("initiator", initiator);
    parser.getValue("pin", pin);
    parser.getValue("participants", participants);
    parser.getValue("existParticipants", existParticipants);
    parser.getValue("audioOnly", audioOnly);
    parser.getValue("autoAnswer", autoAnswer);
    parser.getValue("clientId", clientId);
}

std::string CallAddParticipantMessageContent::digest(const Message &message) const {
    std::string result = getNotificationUserName(initiator, message.conversation.target);

    result += " 邀请 ";


    for (std::list<std::string>::const_iterator it = participants.begin(); it != participants.end(); ++it) {
        const std::string &userId = *it;
        result += " ";
        result += getNotificationUserName(userId, message.conversation.target);
    }
    result += " 加入群通话";

    return result;
}

std::string CallAddParticipantMessageContent::formatNotification(const Message &msg) const {
    return digest(msg);
}

CallAddParticipantMessageContent::~CallAddParticipantMessageContent()
{
}

MessageContentType CallAddParticipantMessageContentPrototype::getType() const
{
    return VOIP_CONTENT_TYPE_ADD_PARTICIPANT;
}

MessageContentPersistFlag CallAddParticipantMessageContentPrototype::getFlag() const
{
    return PersistFlag_PERSIST;
}

MessageContent * CallAddParticipantMessageContentPrototype::productEmptyContent() const
{
    return new CallAddParticipantMessageContent();
}
};
