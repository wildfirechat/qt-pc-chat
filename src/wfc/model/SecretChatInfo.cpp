#include "SecretChatInfo.h"
#include "../utility/JsonTools.h"


namespace WFCLib {
SecretChatInfo::SecretChatInfo() {}

void SecretChatInfo::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    getValue(value, "targetId", targetId);
    getValue(value, "userId", userId);
    int iState;
    getValue(value, "state", iState);
    state = (SecretChatState)iState;
    getValue(value, "burnTime", burnTime);
    getValue(value, "createTime", createTime);
}

void SecretChatInfo::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("targetId");
    writer.String(targetId);

    writer.String("userId");
    writer.String(userId);

    writer.String("state");
    writer.Int((int)state);

    writer.String("burnTime");
    writer.Int(burnTime);

    writer.String("createTime");
    writer.Int64(createTime);

    writer.EndObject();
}
};
