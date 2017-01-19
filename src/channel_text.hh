#ifndef H_DSCPP_CHANNEL_TEXT
#define H_DSCPP_CHANNEL_TEXT

#include "api.hh"
#include "fetchable.hh"

namespace dsc {

class TextChannel : EventEmitter {
    public:
        ~TextChannel();
        TextChannel();
        Error fetch(snowflake id);
        Error fetch(rapidjson::Document v);
        void setHandler(BaseChannelHandler* h);
};

class TextDMChannel : EventEmitter {
    public:
        DMChannel();
        Error fetch(snowflake id);
        Error fetch(rapidjson::Document v);
        void setHandler(TextChannelHandler* h)
};

struct BaseTextChannelHandler : BaseEventHandler {
    void onMessageCreate(TextChannel* emitter, TextMessage* message);
};

}
#endif