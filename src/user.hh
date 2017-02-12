#ifndef H_DSCPP_USER
#define H_DSCPP_USER

#include "api.hh"
#include <time.h>

namespace dsc {

typedef pmask_t uint64_t;
typedef user_status_t int8_t;

enum PermBits : pmask_t {
    CREATE_INSTANT_INVITE   = 0x01 << 1,
    KICK_MEMBERS            = 0x01 << 2,
    BAN_MEMBERS             = 0x01 << 3,
    ADMINISTRATOR           = 0x01 << 4,
    MANAGE_CHANNELS         = 0x01 << 5,
    MANAGE_GUILD            = 0x01 << 6,
    ADD_REACTIONS           = 0x01 << 7,
    MSG_READ                = 0x01 << 8,
    MSG_SEND                = 0x01 << 9,
    MSG_SEND_TTS            = 0x01 << 10,
    MSG_HISTORY_READ        = 0x01 << 14,
    EMBED_LINKS             = 0x01 << 12,
    ATTACH_FILES            = 0x01 << 13,
    VOX_CONNECT             = 0x01 << 15,
    VOX_SPEAK               = 0x01 << 16,
    VOX_MUTE_MEMBERS        = 0x01 << 17,
    VOX_DEAFEN_MEMBERS      = 0x01 << 18,
    VOX_MOVE_MEMBERS        = 0x01 << 19,
    VOX_USE_VAD             = 0x01 << 20,
    NICK_CHANGE             = 0x01 << 21,
    MANAGE_NICKS            = 0x01 << 22,
    MANAGE_ROLES            = 0x01 << 23,
    MANAGE_WEBHOOKS         = 0x01 << 24,
    MANAGE_EMOJI            = 0x01 << 25,
    MANAGE_MESSAGES         = 0x01 << 11,
};

struct User : Pushable {
    struct Presence {
        enum PresenceStatus : user_status_t {
            OFFLINE = -1,
            IDLE,
            ONLINE,
        };
        PresenceStatus stat;
        std::vector<Role> roles;
        std::string game;
    }
    std::string uname, tag, email;
    bool bot, mfa, verified;
    ushort discriminator;
    pmask_t perms;
    
    ~User();
    User();
    WAPIError fetch(snowflake id);
    WAPIError parse(const rapidjson::Document* v);
};

}