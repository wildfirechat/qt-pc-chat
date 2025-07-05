#include "RejectJoinGroupNotificationContent.h"
#include "../utility/JsonTools.h"
#include "Message.h"
#include "../model/Conversation.h"


namespace WFCLib {

const RejectJoinGroupNotificationContentPrototype RejectJoinGroupNotificationContent::sPrototype;

MessagePayload RejectJoinGroupNotificationContent::encode() const {
    MessagePayload payload = NotificationMessageContent::encode();
    JsonBuilder builder;
    builder.setValue("o", operatorId);
    builder.setValue("g", groupId);
    JsonBuilder rejectUserBuilder;
    for(auto it = rejectUser.begin(); it != rejectUser.end(); ++it) {
        rejectUserBuilder.setValue(it->first, it->second);
    }
    builder.setValue("mi", rejectUserBuilder.build());
    payload.binaryContent = builder.build();
    return payload;
}

void RejectJoinGroupNotificationContent::decode(const MessagePayload & payload) {
    NotificationMessageContent::decode(payload);
    JsonParser parser(payload.binaryContent);
    if (!parser.isParsed()) {
        return;
    }

    parser.getValue("o", operatorId);
    parser.getValue("g", groupId);
    parser.getValue("mi", rejectUser);
}

MessageContent* RejectJoinGroupNotificationContent::clone() const {
    RejectJoinGroupNotificationContent *p = new RejectJoinGroupNotificationContent();
    p->extra = extra;
    p->groupId = groupId;
    p->operatorId = operatorId;
    p->rejectUser = rejectUser;
    return p;
}

const MessageContentPrototype* RejectJoinGroupNotificationContent::getPrototype() const {
    return &sPrototype;
}

std::string RejectJoinGroupNotificationContent::digest(const Message &message) const {
    std::string result;
    for(auto it = rejectUser.begin(); it != rejectUser.end(); ++it) {
        std::string name = getNotificationUserName(it->first, message.conversation.target);
        result += name;
        result += " ";
    }

    result += "拒绝加入群组";

    return result;
}

std::string RejectJoinGroupNotificationContent::formatNotification(const Message &msg) const {
    return digest(msg);
}
};
