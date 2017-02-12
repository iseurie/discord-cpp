#ifndef H_DSCPP_CHANNEL_TEXT
#define H_DSCPP_CHANNEL_TEXT

#include "api.hh"
#include "guild.hh"
#include "user.hh"

namespace dsc {

struct TextMessage : Pushable {
    std::string content;
    User author;
    snowflake guild_id;
    
    TextMessage();
    ~TextChannel();
    WAPIError fetch(snowflake id);
    WAPIError parse(const rapidjson::Document* v);
};

struct GuildTextChannel : Pushable {
    struct Overwrite {
        enum OverwriteType { ROLE, MEMBER };
        
        snowflake target;

        OverwiteType t;
        pmask_t allow;
        pmask_t deny;

        rapidjson::Document serialize();
        ~Overwrite();
        Overwrite();
    }

    Overwrite::serialize() {
        rapidjson::Document d;
        d["id"] = target;
        type == MEMBER ? d["type"] = "member" : d["type"] = "role"; 
        d["allow"] = allow;
        d["deny"] = deny;
    }
    std::string topic, name;
    int user_limit, pos;
    snowflake guild_id, lmid;
    std::vector<Overwrite> overwrites;

    ~GuildTextChannel();
    GuildTextChannel();
    WAPIError fetch(Client* c, snowflake id);
    WAPIError parse(const rapidjson::Document* v);
    rapidjson::Document serialize();
};

rapidjson::Document GuildTextChannel::serialize() {
    rapidjson::Document d;
    d["is_private"] = false;
    d["guild_id"] = guild_id;
    d["name"] = name;
    d["type"] = "text";
    d["position"] = pos;
    d["is_private"] = false;
    for(int i = 0; i < overwrites.size(); ++i) {
        d["permission_overwrites"][i] = overwrites[i].serialize();
    }
    d["topic"] = topic;
    d["last_message_id"] = lmid;
    return d;
}

WAPIError fetch()

struct DirectTextChannel : Fetchable {
    User recipient;
    DirectTextChannel();
    WAPIError fetch(snowflake id);
    WAPIError parse(const rapidjson::Document* v);
};

}
#endif