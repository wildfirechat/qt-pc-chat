#ifndef SECRETCHATINFO_H
#define SECRETCHATINFO_H
#include "../utility/Serializable.h"

namespace WFCLib {

enum SecretChatState {
    SecretChatState_Starting,
    SecretChatState_Accepting,
    SecretChatState_Established,
    SecretChatState_Canceled
};

class SecretChatInfo : public Serializable
{
public:
    SecretChatInfo();
    std::string targetId;
    std::string userId;
    SecretChatState state;
    int burnTime;
    int64_t createTime;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};
}
#endif // SECRETCHATINFO_H
