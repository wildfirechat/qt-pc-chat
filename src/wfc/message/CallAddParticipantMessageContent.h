#ifndef CALLADDPARTICIPANTMESSAGECONTENT_H
#define CALLADDPARTICIPANTMESSAGECONTENT_H

#include "NotificationMessageContent.h"
#include "../model/callparticipant.h"

namespace WFCLib {
class CallAddParticipantMessageContentPrototype;
class CallAddParticipantMessageContent : public NotificationMessageContent
{
public:
    CallAddParticipantMessageContent();
    virtual MessagePayload encode() const;
    virtual void decode(const MessagePayload &payload);
    virtual ~CallAddParticipantMessageContent();
    virtual MessageContent* clone() const;
    virtual std::string digest(const Message &message) const;
    virtual std::string formatNotification(const Message &msg) const;
    virtual const MessageContentPrototype* getPrototype() const;
public:
    std::string callId;
    std::string initiator;
    std::string pin;
    std::list<std::string> participants;
    std::list<CallParticipant> existParticipants;
    bool audioOnly;
    bool autoAnswer;
    std::string clientId;

    const static CallAddParticipantMessageContentPrototype sPrototype;
};

class CallAddParticipantMessageContentPrototype : public MessageContentPrototype {
    virtual MessageContentPersistFlag getFlag() const;
    virtual MessageContentType getType() const;
    virtual MessageContent *productEmptyContent() const;
};
};
#endif // CALLADDPARTICIPANTMESSAGECONTENT_H
