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
    RAPIError fetch(snowflake id);
    RAPIError parse(const rapidjson::Document* v);
};

struct GuildTextChannel : Pushable {
    struct Overwrite {
        enum OverwriteType { ROLE, MEMBER };

        OverwiteType t;
        pmask_t allow;
        pmask_t deny;

        ~Overwrite();
        Overwrite();
    }
    std::string topic, name;
    int user_limit, pos;
    snowflake guild_id;
    std::vector<Overwrite> overwrites;

    ~GuildTextChannel();
    GuildTextChannel();
    RAPIError fetch(snowflake id);
    RAPIError parse(const rapidjson::Document* v);
    rapidjson::Document serialize();
};

struct DirectTextChannel : Fetchable {
    User recipient;
    DirectTextChannel();
    RAPIError fetch(snowflake id);
    RAPIError parse(const rapidjson::Document* v);
};

}
#endif