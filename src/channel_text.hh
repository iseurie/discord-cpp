#ifndef H_DSCPP_CHANNEL_TEXT
#define H_DSCPP_CHANNEL_TEXT

#include "api.hh"

namespace dsc {

typedef struct TextChannelHandler : BaseEventHandler {
    
}

class TextChannel : EventEmitter {
    public:
    ~Channel();
    Channel();
    Error fetch(snowflake id);
    Error fetch(rapidjson::Document v);
    void setHandler(BaseChannelHandler* h)
}

class TextDMChannel : EventEmitter {
    public:
    DMChannel();
    Error fetch(snowflake id);
    Error fetch(rapidjson::Document v);
    void setHandler(TextChannelHandler* h)
    void release();
}

}
#endif