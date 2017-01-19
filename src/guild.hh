#ifndef H_DSCPP_GUILD
#define H_DSCPP_GUILD

#include <vector>
#include "api.hh"
#include "channel_text.hh"

namespace dsc {

class Guild : EventEmitter {
    public:
        ~Guild();
        Guild();
        Error fetch(snowflake id);
        Error fetch(rapidjson::Document v);
        void setHandler(GuildHandler* h);
        const std::vector<TextChannel>* getTextChannels();
        const std::vector<User*>* getMembers();
        
    private:
        std::vector<TextChannel> channels_txt;
        std::vector<User*> members;
};

struct BaseGuildHandler : BaseEventHandler {
    
};

}
#endif