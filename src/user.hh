#ifndef H_DSCPP_USER
#define H_DSCPP_USER

#include "api.hh"

namespace dsc {

class User : EventEmitter {
    public:
    ~User();
    User();
    Error fetch(snowflake id);
    Error fetch(rapidjson::Document v);
    void release();
}

typedef struct UserHandler : BaseEventHandler {

}

}