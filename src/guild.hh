#ifndef H_DSCPP_GUILD
#define H_DSCPP_GUILD

#include "api.hh"
#include "channel_text.hh"

namespace dsc {

class Guild : Fetchable {
    private:
    public:
    ~Guild();
    Guild();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    rapidjson::Document serialize();
};

}
#endif