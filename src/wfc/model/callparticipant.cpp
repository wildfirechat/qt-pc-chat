#include "callparticipant.h"
#include "../utility/JsonTools.h"

namespace WFCLib {
void CallParticipant::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    // std::string userId;
    getValue(value, "userId", userId);
    // int64_t userAcceptTime;
    getValue(value, "userAcceptTime", userAcceptTime);
    // int64_t userjoinTime;
    getValue(value, "userjoinTime", userjoinTime);
    // bool videoMuted;
    getValue(value, "videoMuted", videoMuted);
}

void CallParticipant::Serialize(void *pwriter) const  {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);
    writer.StartObject();

    // std::string userId;
    writer.String("userId");
    writer.String(userId);

    // int64_t userAcceptTime;
    writer.String("userAcceptTime");
    writer.Int64(userAcceptTime);

    // int64_t userjoinTime;
    writer.String("userjoinTime");
    writer.Int64(userjoinTime);

    // bool videoMuted;
    writer.String("userjoinTime");
    writer.Bool(videoMuted);

    writer.EndObject();
}
};
