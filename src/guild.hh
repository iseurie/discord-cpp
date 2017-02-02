#ifndef H_DSCPP_GUILD
#define H_DSCPP_GUILD

#include "rapidjson/document.h"
#include "api.hh"
#include "channel_text.hh"
#include <time.h>

namespace dsc {

struct Guild : Pushable {
    struct Member : User {
        time_t joined;
        std::string nick; // where empty, assume none
        std::vector<Role> roles;
        bool deaf, mute;
    }

    std::string name;
    snowflake owner_id, afk_channel_id, embed_channel_id;
    int afk_timeout, verification_level, 
            default_message_notifications, 
            mfa_level, ;
    time_t joined;
    bool embed_enabled, available;
    std::string icon_hash, splash_hash, region;
    std::vector<TextChannel> text_channels;
    std::vector<VoiceChannel> voice_channels;
    std::vector<Role> roles;
    std::vector<Emoji> emojis;
    std::vector<VoiceState> voice_state;
    std::vector<GuildMember> 

    ~Guild();
    Guild();
    ErrorCode fetch(snowflake id, long* err = NULL);
    rapidjson::ParseResult parse(rapidjson::Document v);
    rapidjson::Document serialize();
};

rapidjson::Document Guild::serialize(bool extra = true) {
    using namespace rapidjson;
    Document d;
    d["id"]                 = id;
    d["name"]               = name;
    d["icon"]               = icon_hash;
    d["splash"]             = splash_hash;
    d["owner_id"]           = owner_id;
    d["region"]             = region;
    d["afk_channel_id"]     = afk_channel_id;
    d["afk_timeout"]        = afk_timeout;
    d["embed_enabled"]      = embed_enabled;
    d["embed_channel_id"]   = embed_channel_id;
    d["verification_level"] = verification_level;
    d["default_message_notifications"] = default_message_notifications;
    if(extra) {
        for(int i = 0; i < roles.length(); ++i) {
            d["roles"][i] = (Value)roles[i].serialize();
        } for(int i = 0; i < emojis.length(); ++i) {
            d["emojis"][i] = (Value)emojis[i].serialize();
        } for(int i = 0; i < voice_state.length(); ++i) {
            d["voice_states"][i] = (Value)voice_state[i].serialize();
        } for(int i = 0; i < members.length(); ++i) {
            d["members"][i] = members[i];
        } for(int i = 0; i < channels.length(); )
    }
    return d;
}

}
#endif