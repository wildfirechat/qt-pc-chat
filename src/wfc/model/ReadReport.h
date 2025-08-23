#ifndef READREPORT_H
#define READREPORT_H
#include "../utility/Serializable.h"
#include "Conversation.h"


namespace WFCLib {
class ReadReport : public Serializable
{
public:
    ReadReport();
    Conversation conversation;
    std::string userId;
    int64_t timestamp;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};
}
#endif // READREPORT_H
