#ifndef H_DSCPP_API
#define H_DSCPP_API

#include <stdint.h>
#include "rapidjson/document.h"

namespace dsc {

typedef snowflake uint64_t;
typedef discriminator short;

class Fetchable {
    private:
    snowflake id;
    friend class Client;

    public:
    snowflake getId();
    virtual Error fetch(snowflake id);
    virtual Error parse(rapidjson::Document v);
    bool matches(snowflake id);
};

snowflake Fetchable::getId() {
    return id;
}

bool Fetchable::matches(snowflake id) {
    return id == this.id;
};

enum Error {
    NIL,
    NO_PERM,
};

}
#endif