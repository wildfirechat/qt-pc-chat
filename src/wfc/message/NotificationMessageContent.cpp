//
//  NotificationMessageContent.cpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/31.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#include "NotificationMessageContent.h"
#include "../client/ChatClient.h"
#include "../model/UserInfo.h"

namespace WFCLib {
MessagePayload NotificationMessageContent::encode() const {
    MessagePayload payload = MessageContent::encode();
    return payload;
}

void NotificationMessageContent::decode(const MessagePayload &payload) {
    MessageContent::decode(payload);
}
};
