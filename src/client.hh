#ifndef H_DSCPP_CLIENT
#define H_DSCPP_CLIENT

#include <string>
#include <vector>
#include <stdint.h>
#include "user.hh"

namespace dsc
{

enum ClientType
{
    NORMAL,
    BOT
};

typedef client_scope_t : uint16_t;

enum GatewayOPs : uint8_t
{
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

enum ClientOAUTHScope : client_scope_t
{
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
class Client
{
    //- TODO: clean up CURL instance in destructor
  private:
    friend class Pushable;
    ClientType type;
    char *sessionEndpointUri;
    char *sessionToken;
    char *sessionId;
    client_scope_t scope;
    CURL* curl;
    // to make thread-safe, call curl_global_init()
    void getCurl();

  public:
    BaseEventHandler handler;
    Client();
    ClientType getClientType();
    ErrorCode auth(const char *user, const char *pass);
    ErrorCode auth(const char *token);
    ErrorCode connect();
    ErrorCode resume();
    ErrorCode updateGameStatus(unsigned long idle_since, const char *game);
};

Client::getCurl() {
    if(!curl) curl = curl_easy_init();
    return curl;
}

}
#endif
