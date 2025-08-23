#ifndef CALLPARTICIPANT_H
#define CALLPARTICIPANT_H
#include <string>
#include "../utility/Serializable.h"

namespace WFCLib {
class CallParticipant : public Serializable {
public:
    std::string userId;
    int64_t userAcceptTime;
    int64_t userjoinTime;
    bool videoMuted;

    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *value);
};
};

#endif // CALLPARTICIPANT_H
