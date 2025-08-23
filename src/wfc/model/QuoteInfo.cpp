#include "QuoteInfo.h"
#include "../utility/JsonTools.h"


namespace WFCLib {
QuoteInfo::QuoteInfo() {}

void QuoteInfo::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }
    int64_t messageUid;
    std::string userId;
    std::string userDisplayName;
    std::string messageDigest;

    getValue(value, "messageUid", messageUid);
    getValue(value, "userId", userId);
    getValue(value, "userDisplayName", userDisplayName);
    getValue(value, "messageDigest", messageDigest);
}

void QuoteInfo::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("messageUid");
    writer.Int64(messageUid);

    writer.String("userId");
    writer.String(userId);

    writer.String("userDisplayName");
    writer.String(userDisplayName);

    writer.String("messageDigest");
    writer.String(messageDigest);

    writer.EndObject();
}
};
