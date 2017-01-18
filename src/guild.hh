#ifndef H_DSCPP_GUILD
#define H_DSCPP_GUILD

#include "api.hh"

namespace dsc {

class Guild : EventEmitter {
    ~Guild();
    Guild();
    Error fetch(snowflake id);
    Error fetch(rapidjson::Document v);
    void setHandler(BaseChannelHandler* h)
}

typedef struct GuildHandler : BaseEventHandler {
    
}

}
#endif