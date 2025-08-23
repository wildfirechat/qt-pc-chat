#include "DomainInfo.h"
#include "../utility/JsonTools.h"

namespace WFCLib {
DomainInfo::DomainInfo() {}


void DomainInfo::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    getValue(value, "domainId", domainId);
    getValue(value, "name", name);
    getValue(value, "desc", desc);
    getValue(value, "email", email);
    getValue(value, "tel", tel);
    getValue(value, "address", address);
    getValue(value, "extra", extra);
    getValue(value, "updateDt", updateDt);
}

void DomainInfo::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("updateDt");
    writer.Int64(updateDt);

    writer.String("domainId");
    writer.String(domainId);

    writer.String("name");
    writer.String(name);

    writer.String("desc");
    writer.String(desc);

    writer.String("email");
    writer.String(email);

    writer.String("tel");
    writer.String(tel);

    writer.String("address");
    writer.String(address);

    writer.String("extra");
    writer.String(extra);

    writer.EndObject();
}

}
