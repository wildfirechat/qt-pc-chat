#ifndef MARKUNREADMESSAGECONTENT_H
#define MARKUNREADMESSAGECONTENT_H

#include "MessageContent.h"

namespace WFCLib {


class MarkUnreadMessageContentPrototype;
class MarkUnreadMessageContent : public MessageContent
{
public:
    MarkUnreadMessageContent();

    int64_t messageUid;
    int64_t timestamp;

    const static MarkUnreadMessageContentPrototype sPrototype;

    // MessageContent interface
public:
    MessagePayload encode() const;
    void decode(const MessagePayload &payload);
    MessageContent *clone() const;
    std::string digest(const Message &message) const;
    const MessageContentPrototype *getPrototype() const;
};

class MarkUnreadMessageContentPrototype : public MessageContentPrototype {
    virtual MessageContentPersistFlag getFlag() const { return PersistFlag_NOT_PERSIST; }
    virtual MessageContentType getType() const { return MESSAGE_CONTENT_TYPE_MARK_UNREAD_SYNC; }
    virtual MessageContent *productEmptyContent() const { return new MarkUnreadMessageContent(); }
};
};

#endif // MARKUNREADMESSAGECONTENT_H
