#ifndef USERONLINESTATE_H
#define USERONLINESTATE_H
#include "../utility/Serializable.h"

namespace WFCLib {

class ClientState : public Serializable
{
public:
    ClientState();
    int platform;
    int state; //设备的在线状态，0是在线，1是有session但不在线，其它不在线。
    int64_t lastSeen; //最后可见
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};

class UserCustomState : public Serializable
{
public:
    UserCustomState();
    int state; //0，未设置，1 忙碌，2 离开（主动设置），3 离开（长时间不操作），4 隐身，其它可以自主扩展。
    std::string text;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};

class UserOnlineState : public Serializable
{
public:
    UserOnlineState();
    std::string userId;
    UserCustomState customState;
    std::list<ClientState> clientStates;
    virtual void Serialize(void *writer) const;
    virtual void Unserialize(const void *pvalue);
};
}
#endif // USERONLINESTATE_H
