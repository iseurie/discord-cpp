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
typedef discriminator unsigned short;

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
}

bool Fetchable::matches(snowflake id) {
    return id == this.id;
}

}

#endif