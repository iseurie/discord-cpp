#ifndef H_DSCPP_API
#define H_DSCPP_API

#include <stdint.h>
#include "rapidjson/document.h"

namespace dsc {

typedef snowflake uint64_t;
typedef discriminator short;
typedef utimestamp long;

class Fetchable {
    private:
    snowflake id;
    
    public:
    virtual Fetchable();
    virtual ~Fetchable();
    virtual Error fetch(snowflake id);
    virtual Error fetch(rapidjson::Document v);
};

enum Error {
    NIL,
    NO_PERM,
};

}
#endif