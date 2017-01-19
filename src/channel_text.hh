#ifndef H_DSCPP_CHANNEL_TEXT
#define H_DSCPP_CHANNEL_TEXT

#include "api.hh"
#include "guild.hh"
#include "user.hh"

namespace dsc {

class TextMessage : Fetchable {
    private:
    char* content;
    User* author;
    
    public:
    TextMessage();
    ~TextChannel();
    Error fetch(snowflake id);
    Error fetch(rapidjson::Document v);
    const char* getContent();
};

class TextChannel : Fetchable {
    private:
    Guild* parent;
    
    public:
    ~TextChannel();
    TextChannel();
    Error fetch(snowflake id);
    Error fetch(rapidjson::Document v);
    Guild* getParent();
};

class DMTextChannel : Fetchable {
    private:
    User* peer;
    
    public:
    DMTextChannel();
    Error fetch(snowflake id);
    Error fetch(rapidjson::Document v);
    User* getPeer();
};

}
#endif