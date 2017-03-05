#ifndef H_DSCPP_CHANNEL_TEXT
#define H_DSCPP_CHANNEL_TEXT

#include "api.hh"
#include "guild.hh"
#include "user.hh"
#include <stdio.h>
#include <strings.h>

namespace dsc {

struct Message : WAPIObject {
    std::string content;
    User author;
    snowflake guild_id;

    TextMessage();
    WAPIError fetch(snowflake chid, snowflake id);
    static WAPIError push(const Client* c, bool mkNew = false);
    rapidjson::ParseResult parse(const rapidjson::Document* v);
};

WAPIError Message::push(const Client* c, snowflake chid, bool mkNew = false) {
    char* path;
    sprintf(path, "channels/%llu/messages/%llu", chid, id);
    return wPush(path, marshal(), mkNew, NULL);
}

WAPIError Message::fetch(const Client* c, snowflake chaid, snowflake id) {
    char* path;
    sprintf(path, "channels/%llu/messages/%llu", chid, id);
    WAPIResult r = c->wGet(path);
    return r->error == WErrorCode::OK ? parse(&r.payload) : r->error;
}

struct TextChannel : Pushable {
    WAPIError getMessages(std::vector<Message>* out) const = 0;
    WAPIError getMessage(Message* out) const = 0;
}

struct GuildTextChannel : Pushable {
    

    std::string topic, name;
    int user_limit, pos;
    snowflake guild_id, lmid;
    std::vector<Overwrite> overwrites;

    WAPIError fetch(const Client* c, snowflake id);
    WAPIError parse(const rapidjson::Document* v);
    rapidjson::Document serialize();
};

rapidjson::Document GuildTextChannel::serialize() {
    rapidjson::Document d;
    d["is_private"] = false;
    d["guild_id"] = guild_id;
    d["name"] = name;
    d["type"] = "text";
    d["position"] = pos;
    d["is_private"] = false;
    for(int i = 0; i < overwrites.size(); ++i) {
        d["permission_overwrites"][i] = overwrites[i].serialize();
    }
    d["topic"] = topic;
    d["last_message_id"] = lmid;
    return d;
}

struct DirectTextChannel : Fetchable {
    User recipient;
    DirectTextChannel();
    WAPIError fetch(snowflake id);
    WAPIError parse(const rapidjson::Document* v);
};

}
#endif
