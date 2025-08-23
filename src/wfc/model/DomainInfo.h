#ifndef DOMAININFO_H
#define DOMAININFO_H
#include "../utility/Serializable.h"

namespace WFCLib {
class DomainInfo : public Serializable
{
public:
    DomainInfo();
    std::string domainId;
    std::string name;
    std::string desc;
    std::string email;
    std::string tel;
    std::string address;
    std::string extra;
    int64_t updateDt;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};
}
#endif // DOMAININFO_H
