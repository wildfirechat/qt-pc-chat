//
//  Conversation.hpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/29.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#ifndef Conversation_hpp
#define Conversation_hpp

#include <stdio.h>
#include "../utility/Serializable.h"
#include "../enum/WFEnums.h"

namespace WFCLib {

class Conversation : public Serializable {
public:
    Conversation() : conversationType(Single_Type), line(0) {}
    virtual ~Conversation() {}
    bool operator==(const Conversation& other) const {
        return (conversationType == other.conversationType) &&
               (line == other.line) &&
               (target == other.target);
    }
    ConversationType conversationType;
    int line;
    std::string target;

    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *value);
};
};
#endif /* Conversation_hpp */
