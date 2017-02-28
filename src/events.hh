#ifndef H_DSCPP_EVENTS
#define H_DSCPP_EVENTS

#include "api.hh"
#include "perms.hh"
#include "user.hh"

namespace dsc {

//- Events; partitions "create" payload contents and batch Adds to separate events to simplify
struct EConnect {}; 
struct EDisconnect {};

struct EMessageAdd { Message subject; };
struct EMessageDel { Message subject; };

struct EGuildAdd { 
    struct Meta {
        time_t joined_at;
        bool available;
        std::vector<Guild::Member> members;
        std::vector<TextChannel> text_channels;
        std::vector<VoiceChannel> voice_channels;
        std::vector<User::Presence> presences;
    };
    Meta meta;
    Guild subject;
};
struct EGuildDel { snowflake id; bool available; bool kicked; };
struct EGuildMemberAdd {
    // Guild-specific meta-data.
    snowflake guild_id;
    // The guild member in question.
    Guild::Member subject;
};
struct ESelfVoiceStateUpdate { snowflake channel_id, bool self_mute, self_deaf; };
struct EGuildMemberDel { snowflake guild_id; EGuildMemberAdd member; };
struct EGuildTextChannelAdd { GuildTextChannel subject; };
struct EGuildTextChannelDel { GuildTextChannel subject; };
struct EDirectTextChannelAdd { DirectTextChannel subject; };
struct EDirectTextChannelDel { DirectTextChannel subject; };
struct EGuildBanAdd { snowflake guild_id; User subject; };
struct EGuildBanDel { snowflake guild_id; User subject; };
struct EGuildRoleAdd { snowflake guild_id; Role role; };
struct EGuildRoleDel { snowflake guild_id, role_id; };
struct EGuildEmojiUpd { snowflake guild_id; int emojic; Emoji* emojiv; };
struct EGuildIntegrationsUpd { snowflake guild_id; };

struct BaseEventHandler {
     void onConnect(EConnect) {};
     void onDisconnect(EDisconnect) {};
     
     void onMessageAdd(EMessageAdd) {};
     void onMessageDel(EMessageDel) {};

     void onGuildAdd(EGuildAdd) {};
     void onGuildDel(EguildDel) {};
     void onGuildMemberAdd(EGuildMemberAdd) {};
     void onGuildMemberDel(EGuildMemberDel) {};
     void onGuildTextChannelAdd(EGuildTextChannelAdd) {};
     void onGuildTextChannelDel(EGuildTextChannelDel) {};
     void onGuildBanAdd(EGuildBanAdd) {};
     void onGuildBanDel(EGuildBanDel) {};
     void onGuildMemberAdd(EGuildMemberAdd) {};
     void onGuildMemberDel(EGuildMemberDel) {};
     void onGuildRoleAdd(EGuildRoleAdd) {};
     void onGuildRoleDel(EGuildRoleDel) {};
     void onGuildEmojiUpd(EGuildEmojiUpd) {};
     void onGuildIntegrationsUpd(EGuildEmojiUpd) {};
     void onSelfVoiceStateUpd(ESelfVoiceStateUpdate) {};
};

}

#endif