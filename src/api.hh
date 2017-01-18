#ifndef H_DSCPP_API
#define H_DSCPP_API

#include <stdint.h>
#include "rapidjson/rapidjson.h"

namespace dsc {

typedef snowflake uint64_t;

typedef struct BaseEventHandler {}

class Fetchable {
    private:
    snowflake id;
    public:
    virtual ~Fetchable();
    virtual Fetchable(snowflake id);
    virtual Fetchable(rapidjson::Value v);
    virtual Error fetch();
    virtual void release();
}

class EventEmitter : Fetchable {
    private:
    BaseEventHandler* handler;
    public:
    virtual void setHandler(BaseEventHandler* h);
}

enum Error {
    NIL,
};

}
#endif