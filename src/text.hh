#ifndef H_DSCPP_CHANNEL_TEXT
#define H_DSCPP_CHANNEL_TEXT

#include "api.hh"
#include "guild.hh"
#include "user.hh"

namespace dsc {

class TextMessage : Fetchable {
    private:
    char content;
    User author;
    snowflake guild_id;
    
    public:
    TextMessage();
    ~TextChannel();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    const char* getContent();
    const User* getAuthor;
    const snowflake getGuildId();
};

class GuildTextChannel : Fetchable {
    private:
    char* topic;
    char* name;
    int user_limit, pos;
    snowflake guild_id;
    std::vector<Overwrite> overwrites;
    
    public:
    ~GuildTextChannel();
    GuildTextChannel();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    
    const char* getName();
    int getUserLimit();
    snowflake getGuildId();
    const std::vector<Overwrite>* getOverwrites();
};

class DirectTextChannel : Fetchable {
    private:
    User recipient;
    
    public:
    DirectTextChannel();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    const User* getRecipient();
};

}
#endif