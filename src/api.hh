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

enum RAPIRespCode : unsigned short {
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

class Fetchable {
    private:
    snowflake id;
    friend class Client;

    public:
    snowflake getId();
    virtual RAPIRespCode fetch(snowflake id);
    virtual RAPIRespCode parse(rapidjson::Document v);
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
    bool getRespCode(rapidjson::Document* d, RAPIRespCode* r);
    
    public:
    marshal(char* out);
    // to make thread-safe, call curl_global_init()
    RAPIRespCode push(Client* c, CURLcode* r);
    // to make thread-safe, call curl_global_init()
    RAPIRespCode delete(Client* c, CURLcode* r);
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

bool Pushable::getRespCode(rapidjson::Document* d, RAPIRespCode* r) {
    *r = static_cast<RAPIRespCode>(d["code"].GetInt());
    return true;
}

long Pushable::push(Client* c, RAPIRespCode* r, bool mkNew = false) {
    char* uri;
    RAPIRespCode res;
    buildEndpointUri(c, uri);
    curl_global_init();
    CURL* curl = c->getCurl();
    if(!curl) *r = CURL_INIT_FAILED; return CURL_INIT_FAILED;
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
    auto wfunc = [](void* dat, size_t size, size_t nmemb, void* userp) {
        using namespace rapidjson;
        Document d = new Document();
        ParseResult ok = d.Parse(static_cast<char*>(dat));
        if(!ok) return ok;
        long httpStat;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStat);
        if(httpStat != 200) {
            getRespCode(d, r);
        } else {
            *r = NIL;
        } return size * nmemb;
    };
    curl_easy_setopt(curl, CURLOPT_WRITEFUNC, static_cast<CURLOPT_WRITEFUNCTION_PTR>(&wfunc));
    CURLcode res = curl_easy_perform(curl);
    if(cresp != CURLE_OK) return res;
    curl_easy_cleanup(c->getCurl());
    curl_slist_free(header);
    return NULL;
}

long Pushable::delete(Client* c, RAPIRespCode* r) {
    char* uri;
    buildEndpointUri(c, uri);
    curl_global_init();
    CURL* curl = curl_easy_init();
    if(!curl) return CURL_INIT_FAILED;
    curl_easy_setopt(curl, CURLOPT_URL, uri);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        *r = CURL_PERFORM_FAILED;
        return res;
    }
    long httpStat;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStat);
    if(httpStat != 200) {
        getRespCode(d, r);
    } else {
        *r = NIL;
    }
    curl_easy_cleanup(c->getCurl());
    return res;
}

}
#endif