#ifndef H_DSCPP_GUILD
#define H_DSCPP_GUILD

#include <map>
#include "api.hh"
#include "channel_text.hh"

namespace dsc {

class Guild : Fetchable {
    private:
    const GuildEventHandler* handler;

    public:
    struct GuildBanAddEvent { User* banned; };
    struct GuildBanRmEvent { User* freed; };
    
    struct GuildEventHandler {
        void onBanAdd(GuildBanAddEvent);
        void onBanAdd(GuildBanRmEvent);
    };

    ~Guild();
    Guild();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    
    void setHandler(const GuildEventHandler* h);
    const GuildEventHandler* getHandler();
};

}
#endif