#ifndef FRIEND_H
#define FRIEND_H
#include "../utility/Serializable.h"

namespace WFCLib {
class Friend : public Serializable
{
public:
    Friend();
    std::string userId;
    std::string alias;
    std::string extra;
    int64_t timestamp;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};
}
#endif // FRIEND_H
