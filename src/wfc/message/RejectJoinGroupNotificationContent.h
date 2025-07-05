#ifndef REJECTJOINGROUPNOTIFICATIONCONTENT_H
#define REJECTJOINGROUPNOTIFICATIONCONTENT_H

#include "NotificationMessageContent.h"
#include <map>

namespace WFCLib {

class RejectJoinGroupNotificationContentPrototype;
class RejectJoinGroupNotificationContent : public NotificationMessageContent
{
public:
    RejectJoinGroupNotificationContent() {}
    virtual ~RejectJoinGroupNotificationContent() {}

    virtual MessagePayload encode() const;
    virtual void decode(const MessagePayload &payload);

    virtual MessageContent* clone() const;
    virtual std::string digest(const Message &message) const;
    virtual std::string formatNotification(const Message &msg) const;
    virtual const MessageContentPrototype* getPrototype() const;
public:
    std::string groupId;
    std::string operatorId;
    //reject user, value is reject reason: 1 black list, value 2 stranger.
    std::map<std::string, int> rejectUser;
    const static RejectJoinGroupNotificationContentPrototype sPrototype;
};

class RejectJoinGroupNotificationContentPrototype : public MessageContentPrototype {
    virtual MessageContentPersistFlag getFlag() const { return PersistFlag_PERSIST; }
    virtual MessageContentType getType() const { return MESSAGE_CONTENT_TYPE_REJECT_JOIN_GROUP; }
    virtual MessageContent *productEmptyContent() const { return new RejectJoinGroupNotificationContent(); }
};
}
#endif // REJECTJOINGROUPNOTIFICATIONCONTENT_H
