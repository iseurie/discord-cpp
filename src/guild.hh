#ifndef H_DSCPP_GUILD
#define H_DSCPP_GUILD

#include "rapidjson/document.h"
#include "api.hh"
#include "channel_text.hh"

namespace dsc {

struct Guild : Pushable {
    const char* name;
    snowflake owner_id, afk_channel_id, embed_channel_id;
    int afk_timeout, verification_level, default_message_notifications;
    bool embed_enabled;
    const char* icon_hash, splash_hash, region;
    std::vector<Role> roles;

    ~Guild();
    Guild();
    RAPIRespCode fetch(snowflake id);
    rapidjson::ParseResult parse(rapidjson::Document v);
    rapidjson::Document serialize();
};

rapidjson::Document serialize() {
    using namespace rapidjson;
    Document d = new Document();
    d["id"]                 = id;
    d["name"]               = name;
    d["icon"]               = icon_hash;
    d["splash"]             = splash_hash;
    d["owner_id"]           = owner_id;
    d["region"]             = region;
    d["afk_channel_id"]     = afk_channel_id;
    d["afk_timeout"]        = afk_timeout;
    d["embed_enabled"]      = embed_enabled;
    d["embed_channel_id"]   = embed_channel_id;
    d["verification_level"] = verification_level;
    d["default_message_notifications"] = default_message_notifications;
    roles = std::vector<Role>(0);
    roles.reserve(b.size());
    for(SizeType i = 0; i < roles.length(); ++i) {
        roles[i].
    }
}

}
#endif