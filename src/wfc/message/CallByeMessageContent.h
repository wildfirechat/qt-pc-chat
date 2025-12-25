//
//  CallByeMessageContent.h
//  ChatClient
//
//  Created by Apple on 2025/12/25.
//  Copyright © 2025 WildFireChat. All rights reserved.
//

#ifndef CallByeMessageContent_h
#define CallByeMessageContent_h

#include "MessageContent.h"

namespace WFCLib {

class CallByeMessageContentPrototype;
class CallByeMessageContent : public MessageContent
{
public:
    CallByeMessageContent() : reason(0), inviteMessageUid(0) {}
    virtual ~CallByeMessageContent() {}
    
    virtual MessagePayload encode() const;
    virtual void decode(const MessagePayload &payload);
    
    virtual MessageContent* clone() const;
    virtual std::string digest(const Message &message) const;
    virtual const MessageContentPrototype* getPrototype() const;
    
public:
    std::string callId;
    int reason;
    int64_t inviteMessageUid;
    
    const static CallByeMessageContentPrototype sPrototype;
};

class CallByeMessageContentPrototype : public MessageContentPrototype {
    virtual MessageContentPersistFlag getFlag() const { return PersistFlag_PERSIST_AND_COUNT; }
    virtual MessageContentType getType() const { return VOIP_CONTENT_TYPE_END; }
    virtual MessageContent *productEmptyContent() const { return new CallByeMessageContent(); }
};
};
#endif /* CallByeMessageContent_h */
