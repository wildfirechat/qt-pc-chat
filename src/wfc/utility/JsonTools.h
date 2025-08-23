//
//  JsonTools.hpp
//  ChatClient
//
//  Created by Hao Jia on 2020/1/29.
//  Copyright © 2020 WF Chat. All rights reserved.
//

#ifndef JsonTools_hpp
#define JsonTools_hpp

#include <stdio.h>
#include <string>
#include <list>
#include <map>

#include "../vendor/rapidjson/rapidjson.h"
#include "../vendor/rapidjson/writer.h"
#include "../vendor/rapidjson/prettywriter.h"
#include "../vendor/rapidjson/document.h"
#include "Serializable.h"
using namespace wfchatjson;

namespace WFCLib {

class Serializable;
class ChannelMenu;
class ClientState;
extern std::string base64Decode(const std::string &base64str);
extern std::string base64Encode(const std::string &str);

extern bool getValue(const Value &value, const std::string &tag, int &ret);
#if !defined(Q_OS_LINUX) || (!defined(__LP64__) && !defined(__LP64))
extern bool getValue(const Value &value, const std::string &tag, long &ret);
#endif
extern bool getValue(const Value &value, const std::string &tag, int64_t &ret);
extern bool getValue(const Value &value, const std::string &tag, bool &ret);
extern bool getValue(const Value &value, const std::string &tag, std::string &ret);
extern bool getValue(const Value &value, const std::string &tag, Serializable &ret);
extern bool getValue(const Value &value, const std::string &tag, std::list<std::string> &ret);
extern bool getValue(const Value &value, const std::string &tag, std::list<ChannelMenu> &ret);
extern bool getValue(const Value &value, const std::string &tag, std::list<ClientState> &ret);

class JsonParser {
public:
    JsonParser(const std::string &json);
    ~JsonParser();
    
    bool getValue(const std::string &tag, int &ret);
#if !defined(Q_OS_LINUX) || (!defined(__LP64__) && !defined(__LP64))
    bool getValue(const std::string &tag, long &ret);
#endif
    bool getValue(const std::string &tag, int64_t &ret);
    bool getValue(const std::string &tag, float &ret);
    bool getValue(const std::string &tag, double &ret);
    bool getValue(const std::string &tag, bool &ret);
    bool getValue(const std::string &tag, std::string &ret);
    bool getValue(const std::string &tag, Serializable &ret);
    bool getValue(const std::string &tag, std::list<std::string> &ret);
    bool getValue(const std::string &tag, std::map<std::string, int> &ret);
    
    bool isParsed() const { return parsed; }
private:
    Document value;
    bool parsed;
};


class JsonBuilder {
public:
    JsonBuilder();
    ~JsonBuilder();
    

    void setValue(const std::string &tag, int value);
#if !defined(Q_OS_LINUX) || (!defined(__LP64__) && !defined(__LP64))
    void setValue(const std::string &tag, long value);
#endif
    void setValue(const std::string &tag, float value);
    void setValue(const std::string &tag, double value);
    void setValue(const std::string &tag, int64_t value);
    void setValue(const std::string &tag, bool value);
    void setValue(const std::string &tag, const std::string &value);
    void setValue(const std::string &tag, const Serializable *pValue);
    void setValue(const std::string &tag, const std::list<std::string> &value);
    
    std::string build();
private:
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer;
};

};
#endif /* JsonTools_hpp */
