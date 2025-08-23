#ifndef QUOTEINFO_H
#define QUOTEINFO_H
#include "../utility/Serializable.h"

namespace WFCLib {
class QuoteInfo : public Serializable
{
public:
    QuoteInfo();
    int64_t messageUid;
    std::string userId;
    std::string userDisplayName;
    std::string messageDigest;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};
}
#endif // QUOTEINFO_H
