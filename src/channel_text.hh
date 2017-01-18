#ifndef H_DSCPP_CHANNEL_TEXT
#define H_DSCPP_CHANNEL_TEXT

#include "rapidjson/rapidjson.h"
#include "api.hh"

namespace dsc {

typedef struct TextChannelHandler : BaseEventHandler {
    
}

class TextChannel : EventEmitter {
    public:
    ~Channel();
    Channel(snowflake id);
    Channel(rapidjson::Value v);
    Error fetch();
    void release();
    void setHandler(BaseChannelHandler* h)
}

class TextDMChannel : EventEmitter {
    public:
    DMChannel(snowflake id);
    Channel(rapidjson::Value v);
    Error fetch();
    void setHandler(TextChannelHandler* h)
    void release();
}

}
#endif