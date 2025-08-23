#include "uploadmediaurlentry.h"
#include "../utility/JsonTools.h"

namespace WFCLib {
UploadMediaUrlEntry::UploadMediaUrlEntry() {}

void UploadMediaUrlEntry::Serialize(void *pwriter) const
{
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("uploadUrl");
    writer.String(uploadUrl);

    if(!backupUploadUrl.empty()) {
        writer.String("backupUploadUrl");
        writer.String(backupUploadUrl);
    }

    writer.String("mediaUrl");
    writer.String(mediaUrl);

    writer.String("type");
    writer.Int(type);

    writer.EndObject();
}

void UploadMediaUrlEntry::Unserialize(const void *pvalue)
{
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    getValue(value, "uploadUrl", uploadUrl);
    getValue(value, "backupUploadUrl", backupUploadUrl);
    getValue(value, "mediaUrl", mediaUrl);
    getValue(value, "type", type);
}

}
