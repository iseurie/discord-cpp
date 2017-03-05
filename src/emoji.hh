#ifndef H_DSCPP_EMOJI
#define H_DSCPP_EMOJI

#include "api.hh"

namespace dsc {

struct Emoji : Fetchable {
    std::vector<snowflake> active_roles;
    bool require_colons, is_managed;
    std::string name;
    
    Emoji();
    ~Emoji();
    rapidjson::Document serialize();
    WAPIResult fetch(snowflake id);
    WAPIError parse(const rapidjson::Document* v);
};

rapidjson::Document Emoji::serialize() {
    rapidjson::Document d;
    d["name"] = name;
    d["require_colons"] = require_colons;
    d["is_managed"] = managed;
    for(int i = 0; i < active_roles.size(); ++i) {
        d["active_roles"][i] = active_roles[i];
    }
    return d;
}

}
