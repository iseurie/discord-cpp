#ifndef H_DSCPP_EMOJI
#define H_DSCPP_EMOJI

#include "api.hh"

namespace dsc {

struct Emoji : WAPIObject {
    snowflake id;
    std::vector<snowflake> active_roles;
    bool require_colons, is_managed;
    std::string name;

    rapidjson::Document serialize();
    rapidjson::ParseResult parse(const rapidjson::Document* v);
};

rapidjson::ParseResult parse(const rapidjson::Document* v) {
    id = d["id"].GetLong();
    name = d["name"].GetString();
    require_colons = d["require_colons"].GetBool();
    managed = d["is_managed"].GetBool();
    int arSize = d["active_roles"].Size();
    active_roles.reserve(arSize);
    for(int i = 0; i < arSize; i++) {
        Role parsed;
        rapidjson::ParseResult rpr = parsed.parse(d["active_roles"][i]);
        if(rpr.IsError) return rpr;
        active_roles.insert(i, parsed);
    }
}

rapidjson::Document Emoji::serialize() {
    rapidjson::Document d;
    d["id"] = id;
    d["name"] = name;
    d["require_colons"] = require_colons;
    d["is_managed"] = managed;
    for(int i = 0; i < active_roles.size(); ++i) {
        d["active_roles"][i] = active_roles[i];
    }
    return d;
}

}
