#ifndef H_DSCPP_PUSHABLE
#define H_DSCPP_PUSHABLE

#include "api.hh"
#include "rapidjson/document.h"
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
    //- to make thread-safe, call curl_global_init()
    WAPIError push(Client* c, bool mkNew = false);
    //- to make thread-safe, call curl_global_init()
    WAPIError del(Client* c);
};

WAPIError Pushable::push(Client* c, bool mkNew = false) {
    WAPIError err;
    char* path;
    sprintf(path, "%s/%llu", endpoint_name.c_str(), id);
    char* verb;
    if(mkNew) {
        verb = "POST";
    } else {
        verb = "PATCH";
    }
    const char* payload = marshal();
    return c->mkReq((const char*[]){ path, verb, payload });
}

WAPIError Pushable::del(Client* c) {
    char* path;
    sprintf(path, "%s/%llu", endpoint_name, id);
    char* verb = "DELETE"
    char* payload;
    marshal(payload);
    return c->mkReq((const char*[]){path, verb, payload});
}

}
#endif
