#include "MessageContent.h"
#include "../client/ChatClient.h"
#include "../model/UserInfo.h"

namespace WFCLib {
MessagePayload MessageContent::encode() const {
    MessagePayload payload;
    payload.contentType = getPrototype()->getType();
    payload.extra = extra;
    return payload;
}

void MessageContent::decode(const MessagePayload &payload) {
    extra = payload.extra;
}

std::string MessageContent::getNotificationUserName(const std::string &userId, const std::string &groupId) const {
    if (userId == ChatClient::Instance()->getCurrentUserId()) {
        return "你";
    } else {
        UserInfo userInfo = ChatClient::Instance()->getUserInfo(userId, groupId, false);
        return userInfo.getReadableName();
    }
}
};
