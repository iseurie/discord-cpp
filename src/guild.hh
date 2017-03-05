#ifndef H_DSCPP_GUILD
#define H_DSCPP_GUILD

#include "rapidjson/document.h"
#include "api.hh"
#include "channel_text.hh"
#include "3339.hh"

namespace dsc {

struct Guild : Pushable {
    struct Member : User {
        time_t joined;
        std::string nick; // where empty, assume none
        std::vector<Role> roles;
        bool deaf, mute;
    }

    bool embed_enabled;
    
    std::string icon_hash, splash_hash, region;
    std::vector<TextChannel> text_channels;
    std::vector<VoiceChannel> voice_channels;
    std::vector<Role> roles;
    std::vector<Emoji> emojis;
    std::vector<GuildMember> members;
    int afk_timeout, verification_level, 
            default_message_notifications, 
            mfa_level;
    std::string name;
    snowflake owner_id, afk_channel_id, embed_channel_id;
    
    WAPIError fetch(snowflake id);
    rapidjson::ParseResult parse(rapidjson::Document v);
    rapidjson::Document serialize();
    
    static WAPIError mkChannel(const Client* c,
            snowflake id = this->id, const TextChannel* ch);
    static WAPIError mkChannel(const Client* c,
            snowflake id = this->id, const VoiceChannel* ch);
    static WAPIError mvChannelPos(const Client* c, 
            const std::vector<snowflake>* ids);
    static WAPIError getChannels(const Client* c, 
            snowflake id = this->id,
            std::vector<TextChannel>* text = NULL, 
            std::vector<VoiceChannel>* voice = NULL);
    Guild();
};

static WAPIError Guild::mkChannel(const Client* c, snowflake id, const TextChannel* ch) {
    char* path, payload;
    sprintf(path, "guilds/%llu/channels", id);
    return wPush(c, path, ch->marshal());
}

WAPIError Guild::mkChannel(const Client* c, VoiceChannel ch) {
    mkChannel(c, id, ch);
}

static WAPIError Guild::mkChannel(const Client* c, snowflake id, VoiceChannel ch) {
    char* path, payload;
    sprintf(path, "guilds/%llu/channels", id);
    payload = ch.marshal();
    return wPush(c, path, payload);
}

WAPIError Guild::getChannels(const Client* c, const std::vector<TextChannel>* text, std::vector<VoiceChannel>* voice) {
    getChannels(c, id, text, voice);
}

WAPIError Guild::mvChannelPos(const Client* c, const std::vector<snowflake>* ids) {
    mvChannelPos(c, id, ids);
}

static WAPIError Guild::mvChannelPos(const Client* c, snowflake id, const std::vector<snowflake>* ids) {
    rapidjson::Document d;
    for(int i = 0; i < ids->size(); ++i) {
        d[i]["id"] = ids[i];
        d[i]["position"] = i;
    }
    const char* payload = d.Root().GetString();
    char* path;
    sprintf(path, "guilds/%llu/channels", snowflake);
    c->wPush(path, payload, &d);
}

static WAPIError Guild::getChannels(const Client* c, snowflake id, std::vector<TextChannel>* text, std::vector<VoiceChannel>* voice) {
    #define CHK_WAPI_ERR(e) if(e.code != NIL) return e;
    rapidjson::Document d;
    CHK_WAPI_ERR(c->wGet(params, &d));
    for(int i = 0; i < d.Size(); ++i) {
        if(text && d[i]["type"].GetString() == "text") {
            TextChannel t;
            CHK_WAPI_ERR(t.parse(d[i]));
            text->push_back(t);
        } else if(voice) {
            VoiceChannel v;
            CHK_WAPI_ERR(v.parse(d[i]))
            voice->push_back(v);
        }
    }
}

rapidjson::Document Guild::serialize(Meta* meta = NULL) {
    rapidjson::Document d;
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
    if(meta) {
        char* tstr;
        RfC3339::encode(m->joined_at, NULL, tstr);
        d["joined_at"] = tstr;  
        d["large"] = false; // dummy value
        d["unavailable"] = !m->available;
        d["member_count"] = members.size();
        for(int i = 0;  d < members.size(); ++i) {
            d["voice_states"] = members[i].voice_state.serialize();
        } for(int i = 0; i < roles.length(); ++i) {
            d["roles"][i] = roles[i].serialize();
        } for(int i = 0; i < emojis.length(); ++i) {
            d["emojis"][i] = emojis[i].serialize();
        } for(int i = 0; i < voice_state.length(); ++i) {
            d["voice_states"][i] = members[i].voice_state.serialize();
        } for(int i = 0; i < members.length(); ++i) {
            d["members"][i] = members[i].serialize();
        } for(int i = 0; i < text_channels.length(); ++i) {
            d["channels"][i] = text_channels[i].serialize();
        } for(int i = 0; i < voice_channels.length(); ++i) {
            d["channels"][(i+voice_channels.length())] = voice_channels[i].serialize();
        } for(int i = 0; i < members.length(); ++i) {
            d["presences"] = members[i].presence.serialize();
        }
    }
    return d;
}

}
#endif
