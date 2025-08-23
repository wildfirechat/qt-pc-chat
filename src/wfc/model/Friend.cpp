#include "Friend.h"
#include "../utility/JsonTools.h"


namespace WFCLib {
Friend::Friend() {}

void Friend::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    getValue(value, "userId", userId);
    getValue(value, "alias", alias);
    getValue(value, "extra", extra);
    getValue(value, "timestamp", timestamp);
}

void Friend::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("userId");
    writer.String(userId);

    writer.String("alias");
    writer.String(alias);

    writer.String("extra");
    writer.String(extra);

    writer.String("timestamp");
    writer.Int64(timestamp);

    writer.EndObject();
}
};
