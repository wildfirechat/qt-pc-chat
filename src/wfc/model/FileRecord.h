#ifndef FILERECORD_H
#define FILERECORD_H
#include "../utility/Serializable.h"
#include "Conversation.h"
namespace WFCLib {
class FileRecord : public Serializable
{
public:
    FileRecord();
    Conversation conversation;
    int64_t messageUid;
    std::string userId;
    std::string name;
    std::string url;
    int size;
    int downloadCount;
    int64_t timestamp;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};
}
#endif // FILERECORD_H
