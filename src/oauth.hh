#ifndef H_DSCPP_OAUTH
#define H_DSCPP_OAUTH
#include <map>
#include <stdint.h>
#include <strings.h>

namespace dsc::oauth {

typedef scope_mask_t uint16_t;

// Client OAuth Scope enumeration
// Enumerates the Discord client OAuth scope.
struct Scope : scope_mask_t {
    enum Scopes : scope_mask_t {
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
        RPC_NOTIFY  = 0x01 << 11
        WEBHOOK_INCOMING = 0x01 << 12;
    };
    static const char* marshal(scope_mask_t);
    private:
    static std::map<scope_mask_t, const char*> genStringMap();
    static const std::map stringMap;
};

std::map<int, const char*> Scope::genStringMap() {
    std::map<scope_mask_t, const char*> ret;
    ret.insert(EMAIL,       "email");
    ret.insert(IDENTIFY,    "identify");
    ret.insert(GUILDS,      "guilds");
    ret.insert(JOIN_GUILDS, "guilds.join");
    ret.insert(JOIN_GDM,    "gdm.join");
    ret.insert(RPC,         "rpc");
    ret.insert(RPC_API,     "rpc.api");
    ret.insert(RPC_NOTIFY   "rpc.notifications.read");
    ret.insert(WEBHOOK_INCOMING, "webhook.incoming");
    return ret;
}

const std::map<scope_mask_t, const char*> Scope::stringMap = Scope::genStringMap();

const char* Scope::marshal(scope_mask_t mask) {
    char* ret;
    for(auto& kv : stringMap) {
        if(mask | kv.first) {
            strcat(ret, " ");
            strcat(ret, kv.second);
        }
    } return strdup(ret);
}
#endif
