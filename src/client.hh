#ifndef H_DSCPP_CLIENT
#define H_DSCPP_CLIENT

#include <string>
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "user.hh"

namespace discord {

typedef client_scope_t : uint16_t;

enum GatewayOPs : uint8_t {
    DISPATCH = 0,
    HEARTBEAT,
    IDENTIFY,
    STATUS_UPDATE,
    VOICE_STATE_UPDATE,
    VOICE_SERVER_PING,
    RESUME,
    RECONNECT,
    REQUEST_GUILD_MEMBERS,
    INVALID_SESSION,
    HELLO,
    HEARTBEAT_ACK,
};

// Client OAuth Scope enumeration
// Enumerates the Discord client OAuth scope.
enum ClientOAuthScope : client_scope_t {
    EMAIL       = 0x01 << 1;
    IDENTIFY    = 0x01 << 2;
    BOT         = 0x01 << 3;
    CONNECTIONS = 0x01 << 4;
    GUILDS      = 0x01 << 5;
    JOIN_GUILDS = 0x01 << 6;
    JOIN_GDM    = 0x01 << 7;
    MSG_READ    = 0x01 << 8;
    RPC         = 0x01 << 9;
    RPC_API     = 0x01 << 10;
    WEBHOOK_INCOMING = 0x01 << 11;
};

// Websocket event subscriber
/* <Client> allows the implementation of callbacks to streaming API events, in
 * addition to facilitating internal requests to the Discord RESTful API. It
 * persists no data, leaving the task of caching clients and their IDs to the
 * user for the sake of efficiency and simplicity. Generally, event payloads
 * passing IDs refer only to immediately relevant objects (those already
 * 'created' over the course of a connection's persistence), by virtue of
 * Discord's backend behavior. */
class Client {
    private:
    friend struct WAPIObject;
    ClientType type;
    std::string sessionEndpointUri, sessionToken;
    snowflake session_id[2];
    client_scope_t scope;
    WAPIError doWebReq(const char* dat[3], rapidjson::Document* out = NULL) const;
    WAPIResponse wGet(const char* path) const;
    WAPIError wDel(const char* path, rapidjson::Document* out = NULL) const;
    WAPIError wPush(const char* path, const char* payload, 
            bool mkNew = false, rapidjson::Document* out = NULL) const;
    public:
    enum ClientType { NORMAL, BOT };
    
    ClientType getClientType();
    
    // A <BaseEventHandler>, for the user to assign with callbacks. 
    BaseEventHandler handler;
    // Handles user authentication
    WAPIError auth(const char* user, const char* pass);
    // Handles bot or OAuth bearer authentication
    WAPIError auth(const char* token, ClientType t);
    WAPIError connect();
    WAPIError resume();
    WAPIError hangup();
    WAPIError updateGameStatus(time_t idle_since, const char* game);
    Client();
    ~Client();
};

WAPIError Client::wPush(const char* path, const char* payload,
        bool mkNew = false, rapidjson::Document* out = NULL) {
    const char* verb = mkNew ? "POST" : "PATCH";
    const char* params[] = { path, verb, payload };
    return doWebReq(params, out);
}

WAPIError Client::wDel(const char* path, rapidjson::Document* out = NULL) {
    const char* params[] = { path, "DELETE", NULL };
    return doWebReq(params, out);
}

WAPIResponse Client::wGet(const char* path) {
    WAPIResponse r;
    const char* params[] = { path, "GET", NULL };
    r->error = doWebReq(params, &r.payload);
    return r;
}

// @dat An array of three strings containing, in succession, the request path, verb, and payload.
WAPIError Client::doWebReq(const char* dat[3], rapidjson::Document* out = NULL) {
    #define CHK_CURL_ERR(e, sig) if(e != CURLE_OK) return WAPIError(CURL_sig_FAILED, e);
    struct SWrite {
        rapidjson::Document d;
        WAPIError e;
    };

    CURL* curl = curl_easy_init();
    if(!curl) return CURL_INIT_FAILED;
    CURLcode sig;
    struct curl_slist* header;
    header = curl_slist_append(header, "Content-Type:application/json");
    CHK_CURL_ERR(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header), INIT);
    char* uri;
    sprintf(uri, "http://discordapp.com/%s", dat[0]);
    CHK_CURL_ERR(curl_easy_setopt(curl, CURLOPT_URL, uri), INIT);
    CHK_CURL_ERR(curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, dat[1]), INIT);
    CHK_CURL_ERR(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dat[2]), INIT);
    SWrite resp;
    CHK_CURL_ERR(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp), INIT);

    auto wfunc = [](void* dat, size_t size, size_t nmemb, void* r) {
        SWrite* out = static_cast<SWrite*>(r);
        ParseResult sig = out->d.Parse(static_cast<char*>(dat));
        if(sig.IsError()) {
            out->e = WAPIError(JSON_PARSE_FAILED, ok);
            return size * nmemb;
        }
        short httpStat;
        CHK_CURL_ERR(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStat), INFO);
        if(httpStat < 200 || httpStat > 300) {
            if(d["code"].IsInt()) {
                out->e = WAPIError(static_cast<ErrorCode>(
                        d["code"].GetInt()), httpStat);
            } else {
                out->e = WAPIError(JSON_PARSE_FAILED, httpStat);
            }
        } else {
            out->e = WAPIError(NIL, NULL);
        }
        return size * nmemb;
    };

    typedef size_t(*CURL_WRITEFUNCTION_PTR)(void*, size_t, size_t, void*);
    CHK_CURL_ERR(curl_easy_setopt(curl, CURLOPT_WRITEFUNC,
            static_cast<CURLOPT_WRITEFUNCTION_PTR>(&wfunc)), INIT);
    CHK_CURL_ERR(curl_easy_perform(curl), PERFORM);
    curl_easy_cleanup(curl);
    curl_slist_free(header);
    if(resp.e.code != JSON_PARSE_FAILED) {
        if(out) *out = resp.d;
    } return resp.e;
    #undef CHK_CURL_ERR
}

}
#endif
