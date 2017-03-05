#ifndef H_DSCPP_PUSHABLE
#define H_DSCPP_PUSHABLE

#include "api.hh"
#include "rapidjson/document.h"
#include <strings.h>
#include <curl/curl.h>
#include <strings.h>

namespace dsc {

// Editable API Object
/* <Pushable> objects are editable on the web stack, and deltas
 * to their payloads and state may be 'pushed' via their methods
 * following retrieval. */
struct Pushable : Fetchable {
    virtual const char* marshal() = 0 const;
    WAPIError push(const Client* c, bool mkNew = false);
    WAPIError del(const Client* c);
};

WAPIResult Pushable::push(const Client* c, bool mkNew = false) {
    WAPIError err;
    char* path;
    sprintf(path, "%s/%llu", web_path, id);
    return c->wPush(path, marshal(), mkNew, NULL);
}

WAPIError Pushable::del(const Client* c) {
    char* path;
    sprintf(path, "%s/%llu", endpoint_name, id);
    return c->wDel(path);
}

}
#endif
