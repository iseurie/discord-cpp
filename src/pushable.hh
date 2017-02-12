#ifndef H_DSCPP_PUSHABLE
#define H_DSCPP_PUSHABLE

#include "api.hh"
#include "rapidjson/document.h"
#include <curl/curl.h>

namespace dsc {

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
    void marshal(char* out);
    //- to make thread-safe, call curl_global_init()
    WAPIError push(Client* c, bool mkNew = false);
    //- to make thread-safe, call curl_global_init()
    WAPIError delete(Client* c);
};

void Pushable::marshal(char* out) {
    rapidjson::Document d = serialize();
    StringBuffer buf;
    Writer<StringBuffer> writer(buf);
    d.Accept(writer);
    *out = *buf.GetString();
};


WAPIError Pushable::push(Client* c, bool mkNew = false) {
    WAPIError err;
    char* path;
    sprintf(path, "%s/%llu", endpoint_name, id);
    char* verb;
    if(mkNew) {
        verb = "POST";
    } else {
        verb = "PATCH";
    }
    char* payload;
    marshal(payload);
    return c->mkReq((const char*[]){path, verb, payload});
}

WAPIError Pushable::delete(Client* c) {
    char* path;
    sprintf(path, "%s/%llu", endpoint_name, id);
    char* verb = "DELETE"
    char* payload;
    marshal(payload);
    return c->mkReq((const char*[]){path, verb, payload});
}

}
#endif