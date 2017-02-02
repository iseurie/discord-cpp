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
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
};

struct GuildTextChannel : Fetchable {
    std::string topic, name;
    int user_limit, pos;
    snowflake guild_id;
    std::vector<Overwrite> overwrites;

    ~GuildTextChannel();
    GuildTextChannel();
    RAPIError fetch(snowflake id);
    RAPIError parse(rapidjson::Document v);
};

struct DirectTextChannel : Fetchable {
    User recipient;
    DirectTextChannel();
    RAPIError fetch(snowflake id);
    RAPIError parse(rapidjson::Document v);
    getRecipient();
};

}
#endif