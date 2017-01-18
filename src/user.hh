#ifndef H_DSCPP_USER
#define H_DSCPP_USER

#include "api.hh"
#include "rapidjson/rapidjson.h"

namespace dsc {

class User : EventEmitter {
    public:
    ~User();
    User(snowflake id);
    User(rapidjson::Value v);
    Error fetch();
    void release();
}

struct UserHandler : BaseEventHandler {

}

}