#include "ReadReport.h"
#include "../utility/JsonTools.h"


namespace WFCLib {
ReadReport::ReadReport() {}

void ReadReport::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    int type;
    getValue(value, "conversationType", type);
    conversation.conversationType = (ConversationType)type;
    getValue(value, "target", conversation.target);
    getValue(value, "line", conversation.line);
    getValue(value, "userId", userId);
    getValue(value, "readDt", timestamp);
}

void ReadReport::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    //std::string userId;
    writer.String("userId");
    writer.String(userId);
    //int conversationType;
    writer.String("conversationType");
    writer.Int(conversation.conversationType);
    //std::string target;
    writer.String("target");
    writer.String(conversation.target);
    //int line;
    writer.String("line");
    writer.Int(conversation.line);
    //int64_t readDt;
    writer.String("readDt");
    writer.Int64(timestamp);

    writer.EndObject();
}
};
