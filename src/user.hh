#ifndef H_DSCPP_USER
#define H_DSCPP_USER

#include "api.hh"

namespace dsc {

typedef user_status_t int8_t;

struct User : Pushable {
    struct Presence : Serializable {
        Presence(User* user);
        enum struct PresenceStatus : user_status_t {
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
        enum struct Status : char {
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
