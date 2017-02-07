#ifndef H_DSCPP_EMOJI
#define H_DSCPP_EMOJI

#include "api.hh"

namespace dsc {

struct Emoji : Fetchable {
    std::vector<snowflake> active_roles;
    bool requireColons, managed;
    std::string name;
    
    Emoji();
    ~Emoji();
    RAPIError fetch(snowflake id);
    RAPIError parse(const rapidjson::Document* v);
};

}
#endif