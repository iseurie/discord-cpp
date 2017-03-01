#ifndef H_DSCPP_CHANNEL_TEXT
#define H_DSCPP_CHANNEL_TEXT

#include "api.hh"
#include "guild.hh"
#include "user.hh"

namespace discord {

struct Message : WAPIObject {
    std::string content;
    User author;
    snowflake guild_id;

    TextMessage();

    WAPIResult fetch(snowflake channel_id, snowflake id);
    WAPIError parse(const rapidjson::Document* v);
};

WAPIResult fetch(snowflake channel_id, snowflake id) {
    
}

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
        d["type"] = type == MEMBER ? "member" : "role";
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

struct DirectTextChannel : Fetchable {
    User recipient;
    DirectTextChannel();
    WAPIError fetch(snowflake id);
    WAPIError parse(const rapidjson::Document* v);
};

}
#endif
