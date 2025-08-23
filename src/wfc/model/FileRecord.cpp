#include "FileRecord.h"
#include "../utility/JsonTools.h"


namespace WFCLib {
FileRecord::FileRecord() {}

void FileRecord::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    int type;
    getValue(value, "conversationType", type);
    conversation.conversationType = (ConversationType)type;
    getValue(value, "target", conversation.target);
    getValue(value, "line", conversation.line);
    getValue(value, "messageUid", messageUid);
    getValue(value, "userId", userId);
    getValue(value, "name", name);
    getValue(value, "url", url);
    getValue(value, "size", size);
    getValue(value, "downloadCount", downloadCount);
    getValue(value, "timestamp", timestamp);
}

void FileRecord::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("messageUid");
    writer.Int64(messageUid);

    writer.String("userId");
    writer.String(userId);

    writer.String("conversationType");
    writer.Int(conversation.conversationType);

    writer.String("target");
    writer.String(conversation.target);

    writer.String("line");
    writer.Int(conversation.line);

    writer.String("name");
    writer.String(name);

    writer.String("url");
    writer.String(url);

    writer.String("size");
    writer.Int(size);

    writer.String("downloadCount");
    writer.Int(downloadCount);

    writer.String("timestamp");
    writer.Int64(timestamp);

    writer.EndObject();
}
};
