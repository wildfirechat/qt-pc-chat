// Copyright (c) 2020 WildFireChat. All rights reserved.
#ifndef CallAnswerMessageContent_h
#define CallAnswerMessageContent_h

#include "MessageContent.h"

namespace WFCLib {

class CallAnswerMessageContentPrototype;
class CallAnswerMessageContent : public MessageContent {
public:
    CallAnswerMessageContent() : audioOnly(false), inviteMessageUid(0) {}
    virtual ~CallAnswerMessageContent() {}

    virtual MessagePayload encode() const;
    virtual void decode(const MessagePayload &payload);

    virtual MessageContent* clone() const;
    virtual std::string digest(const Message &message) const;
    virtual const MessageContentPrototype* getPrototype() const;

public:
    std::string callId;
    bool audioOnly;
    int64_t inviteMessageUid;

    const static CallAnswerMessageContentPrototype sPrototype;
};

class CallAnswerMessageContentPrototype : public MessageContentPrototype {
    virtual MessageContentPersistFlag getFlag() const { return PersistFlag_PERSIST_AND_COUNT; }
    virtual MessageContentType getType() const { return VOIP_CONTENT_TYPE_ACCEPT; }
    virtual MessageContent *productEmptyContent() const { return new CallAnswerMessageContent(); }
};

}

#endif
