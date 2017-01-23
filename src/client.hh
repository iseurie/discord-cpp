#ifndef H_DSCPP_CLIENT
#define H_DSCPP_CLIENT

#include <string>
#include <vector>
#include <stdint.h>
#include "user.hh"

namespace dsc {

enum ClientType { NORMAL, BOT };

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

enum ClientOAUTHScope : client_scope_t {
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

class Client {
    private:
    ClientType clientType;
    char* sessionUri;
    char* sessionToken;
    char* sessionId;
    client_scope_t scope;
    User currentUser;
    std::vector<Guild> guilds;
    std::vector<DirectTextChannel> dms;
    public:
    Client();
    Client(bool autoReconnect);
    ClientType getClientType();
    std::vector<DirectTextChannel>* getDMs();
    Error Auth(const char* user, const ch ar* pass);
    Error Auth(const char* token);
};

}
#endif
