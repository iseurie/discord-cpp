#ifndef H_DSCPP_EMOJI
#define H_DSCPP_EMOJI

#include "api.hh"

namespace dsc {

struct Emoji : Fetchable {
    std::vector<snowflake> active_roles;
    bool requireColons, managed;
    char* name;
    
    Emoji();
    ~Emoji();
    ErrorCode fetch(snowflake id, long* err);
    ErrorCode parse(rapidjson::Document v, long* err);
};

}
#endif