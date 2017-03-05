#ifndef H_DSCPP_CHANNEL_TEXT
#define H_DSCPP_CHANNEL_TEXT

#include "api.hh"
#include "guild.hh"
#include "user.hh"
#include <stdio.h>
#include <strings.h>

namespace dsc {

struct Message : Pushable {
    std::string content;
    time_t sent, revised;
    bool pinned, mentions_all;
    std::tuple<std::vector<User>, std::vector<Role>> mentions;
    User author;
    std::vector<User> mentions;
    std::Role mentions;
    snowflake guild_id, channel_id;
    

struct TextMessage : Pushable {
    std::string content;
    time_t timestamp;
    bool pinned;
    User author;
    snowflake guild_id, channel_id;
    Message();
};

WAPIError TextMessage::push(const Client* c, snowflake chid, bool mkNew = false) {
    char* path;
    sprintf(path, "channels/%llu/messages/%llu", chid, id);
    return wPush(path, marshal(), mkNew, NULL);
}

WAPIError TextMessage::fetch(const Client* c, snowflake chid, snowflake id) {
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
    rapidjson::ParseResult parse(const rapidjson::Document* v);
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
    rapidjson::ParseResult parse(const rapidjson::Document* v);
};

}
#endif
