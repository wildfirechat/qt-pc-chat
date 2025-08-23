#include "DeliveryReport.h"
#include "../utility/JsonTools.h"



namespace WFCLib {
DeliveryReport::DeliveryReport() {}

void DeliveryReport::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    getValue(value, "userId", userId);
    getValue(value, "timestamp", timestamp);
}

void DeliveryReport::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("userId");
    writer.String(userId);

    writer.String("timestamp");
    writer.Int64(timestamp);

    writer.EndObject();
}
};
