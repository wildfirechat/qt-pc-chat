// Copyright (c) 2020 WildFireChat. All rights reserved.
#ifndef CallSignalMessageContent_h
#define CallSignalMessageContent_h

#include "MessageContent.h"

namespace WFCLib {

class CallSignalMessageContentPrototype;
class CallSignalMessageContent : public MessageContent {
public:
    CallSignalMessageContent() {}
    virtual ~CallSignalMessageContent() {}

    virtual MessagePayload encode() const;
    virtual void decode(const MessagePayload &payload);

    virtual MessageContent* clone() const;
    virtual std::string digest(const Message &message) const;
    virtual const MessageContentPrototype* getPrototype() const;

public:
    std::string callId;
    std::string signalPayload;

    const static CallSignalMessageContentPrototype sPrototype;
};

class CallSignalMessageContentPrototype : public MessageContentPrototype {
    virtual MessageContentPersistFlag getFlag() const { return PersistFlag_TRANSPARENT; }
    virtual MessageContentType getType() const { return VOIP_CONTENT_TYPE_SIGNAL; }
    virtual MessageContent *productEmptyContent() const { return new CallSignalMessageContent(); }
};

}

#endif
