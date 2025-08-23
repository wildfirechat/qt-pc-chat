#ifndef DELIVERYREPORT_H
#define DELIVERYREPORT_H
#include "../utility/Serializable.h"

namespace WFCLib {
class DeliveryReport : public Serializable
{
public:
    DeliveryReport();
    std::string userId;
    int64_t timestamp;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};
}
#endif // DELIVERYREPORT_H
