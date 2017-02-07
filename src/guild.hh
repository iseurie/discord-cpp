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
    
    // struct <Extra>
    // Data fields sent only in `GUILD_CREATE`.
    struct Extra {
        time_t joined_at;
        bool embed_enabled, available;
        std::string icon_hash, splash_hash, region;
        std::vector<TextChannel> text_channels;
        std::vector<VoiceChannel> voice_channels;
        std::vector<Role> roles;
        std::vector<Emoji> emojis;
        std::vector<VoiceState> voice_state;
        std::vector<GuildMember>;
    };
    
    std::string name;
    snowflake owner_id, afk_channel_id, embed_channel_id;
    int afk_timeout, verification_level, 
            default_message_notifications, 
            mfa_level;
    // - heap-allocated references?
    Extra extra;
    
    ~Guild();
    Guild();
    RAPIError fetch(snowflake id);
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
            d["roles"][i] = (Value)extra.roles[i].serialize();
        } for(int i = 0; i < extra.emojis.length(); ++i) {
            d["emojis"][i] = (Value)extra.emojis[i].serialize();
        } for(int i = 0; i < extra.voice_state.length(); ++i) {
            d["voice_states"][i] = (Value)extra.voice_state[i].serialize();
        } for(int i = 0; i < extra.members.length(); ++i) {
            d["members"][i] = extra.members[i].serialize();
        } for(int i = 0; i < extra.text_channels.length(); ++i) {
            d["channels"][i] = extra.text_channels[i].serialize();
        } for(int i = 0; i < extra.voice_channels.length(); ++i) {
            d["channels"][i] = extra.voice_channels[i].serialize();
        }
    }
    return d;
}

}
#endif