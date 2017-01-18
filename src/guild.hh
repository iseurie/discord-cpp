#ifndef H_DSCPP_GUILD
#define H_DSCPP_GUILD

#include "api.hh"
#include "rapidjson/rapidjson.h"

namespace dsc {

class Guild : EventEmitter {
    ~Guild();
    Guild(snowflake id);
    Guild(rapidjson::Value v);
    Error fetch();
    void release();
    void setHandler(BaseChannelHandler* h)
}

struct GuildHandler : BaseEventHandler {
    
}

}
#endif