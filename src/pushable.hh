#ifndef H_DSCPP_PUSHABLE
#define H_DSCPP_PUSHABLE

#include "api.hh"
#include "rapidjson/document.h"
#include <curl/curl.h>

namespace dsc {

class Pushable : Fetchable {
    private:
    typedef size_t(*CURL_WRITEFUNCTION_PTR)(void*, size_t, size_t, void*);
    
    rapidjson::Document serialize();
    const char* endpoint_name;
    void buildEndpointUri(char* out);
    bool getErrCode(rapidjson::Document* d, ErrorCode* r);
    
    public:
    marshal(char* out);
    // to make thread-safe, call curl_global_init()
    ErrorCode push(Client* c, long* err);
    // to make thread-safe, call curl_global_init()
    ErrorCode delete(Client* c, long* err);
};

void Pushable::marshal(char* out) {
    using namespace rapidjson;
    Document d = serialize();
    StringBuffer buf;
    Writer<StringBuffer> writer(buf);
    d.Accept(writer);
    out = buf.GetString();
};


void Pushable::buildEndpointUri(Client* c, char* out) {
    sprintf(out, "%s/%s/llu", 
            c->sessionEndpointUri, endpoint_name, id);
}

ErrorCode Pushable::getErrCode(rapidjson::Document* d) {
    if(!d["code"].IsInt()) return JSON_PARSE_FAILED;
    return static_cast<ErrorCode>(d["code"].GetInt());
}

ErrorCode Pushable::push(Client* c, long* err, bool mkNew = false) {
    char* uri;
    buildEndpointUri(c, uri);
    CURL* curl = c->getCurl();
    CURLcode res;
    if(!curl) return CURL_INIT_FAILED;
    if(res = curl_easy_setopt(curl, CURLOPT_URL, uri) != CURLE_OK) {
        *err = res;
        return CURL_INIT_FAILED;
    }
    if(mkNew) {
        if(res = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST")) {
            *err = res;
            return CURL_INIT_FAILED;
        }
    } else {
        if(res = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH") != CURLE_OK) {
            *err = res;
            return CURL_INIT_FAILED;
        }
    }
    struct curl_slist* header;
    header = curl_slist_append(header, "Content-Type:application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
    char* payload;
    marshal(payload);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
    ErrorCode ret;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    auto wfunc = [](void* dat, size_t size, size_t nmemb, void* ret) {
        using namespace rapidjson;
        Document d = new Document();
        ParseResult ok = d.Parse(static_cast<char*>(dat));
        if(!ok) {
            *err = ok;
            *ret = JSON_PARSE_FAILED;
        }
        long httpStat;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStat);
        if(httpStat > 299) {
            getErrCode(d, ret);
            *err = httpStat;
        } else {
            *ret = NIL;
        } return size * nmemb;
    };
    curl_easy_setopt(curl, CURLOPT_WRITEFUNC, static_cast<CURLOPT_WRITEFUNCTION_PTR>(&wfunc));
    CURLcode res = curl_easy_perform(curl);
    if(cresp != CURLE_OK) return res;
    curl_easy_cleanup(c->getCurl());
    curl_slist_free(header);
    return NIL;
}

ErrorCode Pushable::delete(Client* c, long* err) {
    char* uri;
    buildEndpointUri(c, uri);
    CURL* curl = curl_easy_init();
    if(!curl) return CURL_INIT_FAILED;
    CURLcode res;
    if(res = curl_easy_setopt(curl, CURLOPT_URL, uri) != CURLE_OK) {
        *err = res;
        return CURL_INIT_FAILED;
    }
    if(res = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE")) {
        *err = res;
        return CURL_INIT_FAILED;
    }
    if(res = curl_easy_perform(curl) != CURLE_OK) {
        *err = res;
        return CURL_PERFORM_FAILED;
    }
    long httpStat;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStat);
    curl_easy_cleanup(c->getCurl());
    if(300 <= httpStat < 200) {
        *err = httpStat;
        return getRespCode(d, err);
    }
    return NIL;
}

}
#endif