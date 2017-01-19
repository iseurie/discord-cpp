#ifndef H_DSCPP_GUILD
#define H_DSCPP_GUILD

#include <map>
#include "api.hh"
#include "channel_text.hh"

namespace dsc {

class Guild : Fetchable {
    private:
    std::map<snowflake, TextChannel> channels_txt;
    std::map<snowflake, User> members;
    std::map<snowflake, std::vector<Role>> roles; 
    
    public:
    ~Guild();
    Guild();
    Error fetch(snowflake id);
    Error fetch(rapidjson::Document v);
    const std::map<snowflake, TextChannel>* getTextChannels();
    const std::map<snowflake, User>* getMembers();
    const std::map<snowflake, std::vector<Role>> getRoles();
};

}
#endif