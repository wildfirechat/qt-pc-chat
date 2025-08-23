#ifndef CHANNELMENU_H
#define CHANNELMENU_H
#include "../utility/Serializable.h"

namespace WFCLib {
class ChannelMenu : public Serializable
{
public:
    ChannelMenu();
    std::string menuId;
    std::string type;
    std::string name;
    std::string key;
    std::string url;
    std::string mediaId;
    std::string articleId;
    std::string appId;
    std::string appPage;
    std::string extra;
    std::list<ChannelMenu> subMenus;

    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};
}
#endif // CHANNELMENU_H
