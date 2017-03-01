#ifndef H_DSCPP_USER
#define H_DSCPP_USER

#include "api.hh"

namespace discord {

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

// Editable Role object
/* The Guild Role stores a permissions bitmask,
 * in addition to a name and various data
 * for its display in the client roster. */
struct Role : Pushable {
    std::string name;
    pmask_t perms;
    bool hoist, mentionable, managed;
    int pos, color_hex;

    WAPIError fetch(snowflake id);
    WAPIError parse(const rapidjson::Document* v);
    ~Role();
    Role();
};

struct User : Pushable {
    struct Presence : Serializable {
        Presence(User* user);
        enum PresenceStatus : user_status_t {
            OFFLINE = -1,
            IDLE,
            ONLINE,
        }; PresenceStatus status;
        std::vector<Role> roles;
        std::string game;
        rapidjson::Document serialize();
        User* user;
    }; Presence presence;

    struct VoiceState : Serializable {
        enum Status : char {
            SELF_MUTE   = 0x01,
            SELF_DEAF   = 0x01 >> 1,
            MUTE        = 0x01 >> 2,
            DEAF        = 0x01 >> 3,
            SUPPRESS    = 0x01 >> 4,
        };
        Status status;
        snowflake user_id, guild_id, channel_id, session_id;
        snowflake session_id[2]; // bit-width 128
        rapidjson::Document serialize();
    }; VoiceState voice_state;

    std::string uname, tag, email;
    bool bot, mfa, verified;
    ushort discriminator;
    pmask_t perms;
    
    ~User();
    User();
    WAPIResult fetch(snowflake id);
    WAPIError parse(const rapidjson::Document* v);
};

<<<<<<< HEAD
rapidjson::Document User::Presence::serialize() {
    rapidjson::Document d;
    d["user"] = *user;
    d["game"] = game;
    d["guild_id"] = guild_id;
    switch(status) {
        case OFFLINE: d["status"] = "offline"; break;
        case IDLE: d["status"] = "idle"; break;
        case ONLINE: d["status"] = "online"; break;
    }
    for(int i = 0; i < user.roles.size(); ++i) {
        d["roles"][i] = user.roles[i].serialize();
    }
}

rapidjson::Document User::VoiceState::serialize() {
    rapidjson::Document d;
    d["guild_id"] = guild_id;
    d["channel_id"] = channel_id;
    d["user_id"] = user_id;
    d["session_id"] = session_id;

    d["deaf"] = status | DEAF;
    d["mute"] = status | MUTE;
    d["self_mute"] = status | SELF_MUTE;
    d["self_deaf"] = status | SELF_DEAF;
    d["suppress"] = status | SUPPRESS;
}

}
=======
}
>>>>>>> 298ea1c469137988cca81bf8c25a1284b90f7736
