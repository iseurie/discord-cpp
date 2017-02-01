#ifndef H_DSCPP_API
#define H_DSCPP_API


#include <stdint.h>
#include <stdio.h>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
namespace dsc {

typedef snowflake uint64_t;
typedef discriminator short;

enum ErrorCode : unsigned short {
    OK                      = 0,
    CURL_INIT_FAILED,
    CURL_PERFORM_FAILED,
    JSON_PARSE_FAILED,

    UNKNOWN_ACCOUNT         = 1*10^4+1,
    UNKNOWN_APPLICATION,
    UNKNOWN_CHANNEL,
    UKNOWN_GUILD,
    UNKNOWN_INTEGRATION,
    UNKNOWN_INVITE,
    UKNOWN_MEMBER,
    UNKNOWN_MESSAGE,
    UNKNOWN_OVERWRITE,
    UNKNOWN_PROVIDER,
    UNKNOWN_ROLE,
    UNKNOWN_USER,
    UNKNOWN_EMOJI,
    
    BOTS_NOT_PERMITTED      = 2*10^4+1,
    BOTS_ONLY_PERMITTED,
    
    MAX_GUILDS_REACHED      = 3*10^4+1,
    MAX_FRIENDS_REACHED,
    MAX_PINS_REACHED,
    MAX_GUILD_ROLES_REACHED,
    TOO_MANY_REACTIONS,
    
    UNAUTHORIZED            = 4*10^4+1,
    
    MISSING_ACCESS          = 5*10^4+1,
    INVALID_ACCOUNT_TYPE,
    CANNOT_EXECUTE_ON_DM,
    EMBED_DISABLED,
    CANNOT_EDIT_O_USR_MSG,
    CANNOT_SEND_EMPTY_MSG,
    CANNOT_SEND_MSG_TO_USR,
    CANNOT_SEND_MSG_IN_VOICE_CHANNEL,
    CHANNEL_VERIFICATION_LVL_TOO_HIGH,
    OAUTH_APP_DOES_NOT_HAVE_BOT,
    OAUTH_APP_LIMIT_REACHED,
    INVALID_OAUTH_STATE,
    MISSING_PERMISSIONS,
    INVALID_AUTH_TOKEN,
    NOTE_TOO_LONG,
    INVALID_MSG_COUNT,
    MSG_PIN_CHANNEL_INVALID,
    MSG_TOO_OLD,

    REACTION_BLOCKED        = 9*10^4+1
};

struct Fetchable {
    snowflake id;

    virtual ErrorCode fetch(snowflake id, long* err);
    virtual ErrorCode parse(rapidjson::Document v, long* err);
    bool matches(snowflake id);
};

snowflake Fetchable::getId() {
    return id;
};

bool Fetchable::matches(snowflake id) {
    return id == this.id;
};

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
    curl_global_init();
    CURL* curl = c->getCurl();
    if(!curl) return CURL_INIT_FAILED;
    curl_easy_setopt(curl, CURLOPT_URL, uri);
    if(mkNew) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    } else {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
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
    curl_global_init();
    CURL* curl = curl_easy_init();
    if(!curl) return CURL_INIT_FAILED;
    curl_easy_setopt(curl, CURLOPT_URL, uri);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        *err = res;
        return CURL_PERFORM_FAILED;
    }
    long httpStat;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStat);
    if(httpStat != 200) {
        *err = httpStat;
        return getRespCode(d, err);
    }
    curl_easy_cleanup(c->getCurl());
    return NIL;
}

}
#endif