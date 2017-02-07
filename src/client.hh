#ifndef H_DSCPP_CLIENT
#define H_DSCPP_CLIENT

#include <string>
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "user.hh"

namespace dsc {

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
    EMAIL       = 0x01 >> 1;
    IDENTIFY    = 0x01 >> 2;
    BOT         = 0x01 >> 3;
    CONNECTIONS = 0x01 >> 4;
    GUILDS      = 0x01 >> 5;
    JOIN_GUILDS = 0x01 >> 6;
    JOIN_GDM    = 0x01 >> 7;
    MSG_READ    = 0x01 >> 8;
    RPC         = 0x01 >> 9;
    RPC_API     = 0x01 >> 10;
    WEBHOOK_INCOMING = 0x01 >> 11;
};

// Websocket event subscriber
/* <Client> allows the implementation of callbacks to streaming 
 * low-level API events. It persists no data, leaving the task of caching 
 * clients and their IDs to the user for the sake of speed and simplicity. 
 * Generally, event payloads passing IDs refer only to immediately relevant 
 * objects (those already 'created' over the course of a connection's 
 * persistence), by virtue of Discord's design. Should the user fail to cache
 * such an object, they may choose to retrieve it using an instance of its type's 
 * respective <Fetchable>.
 */
class Client {
    private:
    friend class Pushable;
    ClientType type;
    std::string sessionEndpointUri, sessionToken;
    snowflake session_id[2];
    client_scope_t scope;
    RAPIError mkReq(const char* dat[3], rapidjson::Document* out);

    public:
    enum ClientType { NORMAL, BOT };
    
    ClientType getClientType();
    
    // A <BaseEventHandler>, for the user to assign with callbacks. 
    BaseEventHandler handler;
    // Handles user authentication
    RAPIError auth(const char *user, const char *pass);
    // Handles bot or OAuth bearer authentication
    RAPIError auth(const char *token, ClientType t);
    RAPIError connect();
    RAPIError resume();
    RAPIError hangup();
    RAPIError updateGameStatus(time_t idle_since, const char *game);
    Client();
    ~Client();
};


// @dat An array of three strings containing, in succession, the request path, verb, and payload.
RAPIError Client::mkReq(const char* dat[3], rapidjson::Document* out = NULL) {
    #define CHK_CURL_ERR(e) if(e != CURLE_OK) return RAPIError(CURL_INIT_FAILED, e);
    struct SWrite {
        rapidjson::Document d;
        RAPIError e;
    };

    CURL* curl = curl_easy_init();
    if(!curl) return CURL_INIT_FAILED;
    CURLcode sig;
    struct curl_slist* header;
    header = curl_slist_append(header, "Content-Type:application/json");
    CHK_CURL_ERR(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header));
    CHK_CURL_ERR(curl_easy_setopt(curl, CURLOPT_URL, uri));
    CHK_CURL_ERR(curl, CURLOPT_CUSTOMREQUEST, dat[1]);
    CHK_CURL_ERR(curl, CURLOPT_POSTFIELDS, dat[2]);
    SWrite resp;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);

    auto wfunc = [](void* dat, size_t size, size_t nmemb, void* r) {
        SWrite* out = static_cast<SWrite*>(r);
        ParseResult sig = out->d.Parse(static_cast<char*>(dat));
        if(sig.IsError()) {
            out->e = RAPIError(JSON_PARSE_FAILED, ok);
            return size * nmemb;
        }
        short httpStat;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStat);
        if(httpStat < 200 || httpStat > 300) {
            if(d["code"].IsInt()) {
                out->e = RAPIError(static_cast<ErrorCode>(d["code"].GetInt()), NULL);
            } else {
                out->e = RAPIError(JSON_PARSE_FAILED, httpStat);
            }
        } else {
            out->e = RAPIError(NIL, NULL);
        }
        return size * nmemb;
    };

    typedef size_t(*CURL_WRITEFUNCTION_PTR)(void*, size_t, size_t, void*);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNC, static_cast<CURLOPT_WRITEFUNCTION_PTR>(&wfunc));
    if(sig = curl_easy_perform(curl) != CURLE_OK) {
        return RAPIError(CURL_PERFORM_FAILED, sig);
    }
    curl_easy_cleanup(curl);
    curl_slist_free(header);
    if(resp.e.code != JSON_PARSE_FAILED) {
        if(out) *out = resp.d;
    } return resp.e;
    #undef CHK_CURL_ERR
}

}
#endif
