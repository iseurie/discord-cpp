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
    
    public:
    TextMessage();
    ~TextChannel();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    const char* getContent();
    const User* getAuthor;
};

class TextChannel : Fetchable {
    private:
    char* name, topic;
    int pos;
    bool private;
    std::vector<Overwrite> overwrites;

    public:
    virtual ~TextChannel();
    virtual TextChannel();
    virtual Error fetch(snowflake id);
    virtual Error parse(rapidjson::Document v);
};

class GuildTextChannel : TextChannel {
    private:
    Guild* parent;
    
    public:
    ~TextChannel();
    TextChannel();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    Guild* getParent();
};

class DirectTextChannel : TextChannel {
    private:
    User* peer;
    
    public:
    DirectTextChannel();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    const User* getPeer();
};

}
#endif