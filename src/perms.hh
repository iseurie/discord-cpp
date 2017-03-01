#define H_DSCPP_PERMS
namespace discord::perms {

typedef pmask_t uint64_t;

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
struct Role : WAPIObject {
    std::string name;
    pmask_t perms;
    bool hoist, mentionable, managed;
    int pos, color_hex;

    Role();
    WAPIError push(const Client* c, snowflake gid, bool mkNew = false);
    WAPIError fetch(const Client* c, snowflake gid, snowflake id, Role* out);
    rapidjson::ParseResult parse(const rapidjson::Document* v);
    rapidjson::Document serialize();
};



struct Overwrite : WAPIObject {
    enum OverwriteType { ROLE, MEMBER };
        
    snowflake target_id;

    OverwiteType t;
    pmask_t allow;
    pmask_t deny;

    rapidjson::ParseResult parse(const rapidjson::Document* v);
    rapidjson::Document serialize();
}

Overwrite::serialize() {
    rapidjson::Document d;
    d["id"] = target_id;
    d["type"] = type == MEMBER ? "member" : "role";
    d["allow"] = allow;
    d["deny"] = deny;
}

Overwrite::parse() {
    rapidjson::Document d;
    target_id = d["id"];
    type = d["type"] == "member" ? MEMBER : ROLE;
    allow = d["allow"];
    deny = d["deny"];
}
