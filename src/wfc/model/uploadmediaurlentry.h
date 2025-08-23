#ifndef UPLOADMEDIAURLENTRY_H
#define UPLOADMEDIAURLENTRY_H

#include <stdio.h>
#include <stdint.h>
#include "../utility/Serializable.h"

namespace WFCLib {

class UploadMediaUrlEntry : public Serializable {
public:
    UploadMediaUrlEntry();
    std::string uploadUrl;
    std::string backupUploadUrl;
    std::string mediaUrl;
    int type;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *value);
};

};
#endif // UPLOADMEDIAURLENTRY_H
