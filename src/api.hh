#ifndef H_DSCPP_API
#define H_DSCPP_API

#include <stdint.h>
#include "rapidjson/document.h"

namespace dsc {

typedef snowflake uint64_t;

typedef struct BaseEventHandler {}

class Fetchable {
    private:
    snowflake id;
    public:
    virtual Fetchable();
    virtual ~Fetchable();
    virtual Error fetch(snowflake id);
    virtual Error fetch(rapidjson::Document v);
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