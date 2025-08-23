#include "UserOnlineState.h"
#include "../utility/JsonTools.h"


namespace WFCLib {
ClientState::ClientState() {}

void ClientState::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    getValue(value, "platform", platform);
    getValue(value, "state", state);
    getValue(value, "lastSeen", lastSeen);
}

void ClientState::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("platform");
    writer.Int(platform);

    writer.String("state");
    writer.Int(state);

    writer.String("lastSeen");
    writer.Int64(lastSeen);

    writer.EndObject();
}

UserCustomState::UserCustomState() {}

void UserCustomState::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    getValue(value, "state", state);
    getValue(value, "text", text);
}

void UserCustomState::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("state");
    writer.Int(state);

    writer.String("text");
    writer.String(text);

    writer.EndObject();
}

UserOnlineState::UserOnlineState() {}

void UserOnlineState::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    getValue(value, "userId", userId);
    getValue(value, "customState", customState);
    getValue(value, "clientStates", clientStates);
}

void UserOnlineState::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("userId");
    writer.String(userId);

    writer.EndObject();
}
};
