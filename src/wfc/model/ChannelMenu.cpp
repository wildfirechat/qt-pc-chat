#include "ChannelMenu.h"
#include "../utility/JsonTools.h"


namespace WFCLib {

ChannelMenu::ChannelMenu() {}

void ChannelMenu::Unserialize(const void *pvalue) {
    const Value& value = *((const Value *)pvalue);
    if (!value.IsObject()) {
        return;
    }

    getValue(value, "menuId", menuId);
    getValue(value, "type", type);
    getValue(value, "name", name);
    getValue(value, "key", key);
    getValue(value, "url", url);
    getValue(value, "mediaId", mediaId);
    getValue(value, "articleId", articleId);
    getValue(value, "appId", appId);
    getValue(value, "appPage", appPage);
    getValue(value, "extra", extra);
    getValue(value, "subMenus", subMenus);
}

void ChannelMenu::Serialize(void *pwriter) const {
    Writer<StringBuffer> &writer = *((Writer<StringBuffer> *)pwriter);

    writer.StartObject();

    writer.String("menuId");
    writer.String(menuId);

    writer.String("type");
    writer.String(type);

    writer.String("name");
    writer.String(name);

    writer.String("key");
    writer.String(key);

    writer.String("url");
    writer.String(url);

    writer.String("mediaId");
    writer.String(mediaId);

    writer.String("articleId");
    writer.String(articleId);

    writer.String("appId");
    writer.String(appId);

    writer.String("appPage");
    writer.String(appPage);

    writer.String("extra");
    writer.String(extra);

    if (!subMenus.empty()) {
        writer.String("subMenus");
        writer.StartArray();
        for (std::list<ChannelMenu>::const_iterator it = subMenus.begin(); it != subMenus.end(); ++it) {
            it->Serialize(pwriter);
        }
        writer.EndArray();
    }

    writer.EndObject();
}
}
