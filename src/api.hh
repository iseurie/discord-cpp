#ifndef H_DSCPP_API
#define H_DSCPP_API

#include <stdint.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace dsc {

// Used to store Discord object IDs
typedef snowflake uint64_t;
// Used to store Discord user discriminators
typedef discriminator unsigned short;

// API error code enumeration
/* This enum contains Discord error codes consistent across
 * the web stack, in addition to a few enumerations for internal
 * failures of the client-side implementation. */
enum struct WErrorCode : unsigned short {
    OK                      = 0,
    CURL_INIT_FAILED,
    CURL_PERFORM_FAILED,
    CURL_INFO_FAILED,
    JSON_PARSE_FAILED,

    UNKNOWN_ACCOUNT         = 1*10000+1,
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
    
    BOTS_NOT_PERMITTED      = 2*10000+1,
    BOTS_ONLY_PERMITTED,
    
    MAX_GUILDS_REACHED      = 3*10000+1,
    MAX_FRIENDS_REACHED,
    MAX_PINS_REACHED,
    MAX_GUILD_ROLES_REACHED,
    TOO_MANY_REACTIONS,
    
    UNAUTHORIZED            = 4*10000+1,
    
    MISSING_ACCESS          = 5*10000+1,
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

    REACTION_BLOCKED        = 9*10000+1
};

struct WAPIError {
    WErrorCode code;
    union USig {
        rapidjson::ParseError parsing;
        CURLcode curl;
        short http;
    };
    USig sig;

    WAPIError(WErrorCode _code, USig _sig):code(_code), sig(_sig){}
    WAPIError(rapidjson::ParseResult r);
};

WAPIError(rapidjson::ParseResult r) {
    this->code = r.IsError() ? WErrorCode::JSON_PARSE_FAILED : WErrorCode::OK;
    this->sig = r;
}

struct WAPIResponse {
    WAPIError error;
    rapidjson::Document payload;
}

struct WAPIObject {
    virtual rapidjson::Document serialize();
    virtual rapidjson::ParseResult parse(const rapidjson::Document* v);
}

WAPIObject::matches(snowflake id) {
    return this->id == id;
}

const char* WAPIObject::marshal() {
    using namespace rapidjson;
    StringBuffer buf;
    buf.Clear();
    Writer<StringBuffer> writer(buf);
    serialize().Accept(writer);
    return strdup(buf.GetString());
};

// struct <Fetchable>
/* <Fetchable> is an interface to objects which can be retrieved by ID,
 * or 'fetched,' directly from the Discord RESTful backend. */
struct Fetchable : WAPIObject {
    snowflake id;
    virtual WAPIError fetch(const Client* c, snowflake id) = 0;
};

}

#endif
