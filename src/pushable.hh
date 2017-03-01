#ifndef H_DSCPP_PUSHABLE
#define H_DSCPP_PUSHABLE

#include "api.hh"
#include "rapidjson/document.h"
#include <strings.h>
#include <curl/curl.h>
#include <strings.h>

namespace discord {

// Editable API Object
/* <Pushable> objects are editable on the web stack, and deltas
 * to their payloads and state may be 'pushed' via their methods
 * following retrieval. */
class Pushable : Fetchable {
    private:
    std::string endpoint_name;
    void buildEndpointUri(char* out);
    bool getErrCode(rapidjson::Document* d, ErrorCode* r);
    
    public:
    const char* marshal();
    WAPIError push(Client* c, bool mkNew = false);
    WAPIError del(Client* c);
};

WAPIError Pushable::push(Client* c, bool mkNew = false) {
    WAPIError err;
    char* path;
   sprintf(path, "%s/%llu", endpoint_name, id);
    char* payload = marshal();
    return c->wPush(path, payload, mkNew, NULL);
}

WAPIError Pushable::del(Client* c) {
    char* path;
    sprintf(path, "%s/%llu", endpoint_name, id);
    return c->wDel(path);
}

}
#endif
