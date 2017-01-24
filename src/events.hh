#ifndef H_DSCPP_EVENTS
#define H_DSCPP_EVENTS

#include "api.hh"

namespace dsc {
    struct EventDirectTextChannel {
        DirectTextChannel* channel;
    };
    struct DirectTextChannelCreate : EventDirectTextChannel {};
    struct DirectTextChannelDelete : EventDirectTextChannel {};
    struct EventGuildTextChannel {
        GuildTextChannel* channel;
    };
    struct GuildTextChannelCreate : EventGuildTextChannel {};
    struct GuildTextChannelUpdate : EventGuildTextChannel {};
    struct GuildTextChannelDelete : EventGuildTextChannel {};

    struct MessageCreateEvent {
        Message* message;
    };

    struct VoiceStatusUpdate {
        snowflake guild_id;
        snowflake channel_id;
        bool self_mute;
        bool self_deaf;
    };
    struct GuildUpdateEvent {
        Guild* guild;
    };
    struct GuildAddEvent : GuildAddEvent{};
    struct GuildDeleteEvent {
        snowflake guild_id;
        bool kicked;
    };
    struct GuildBanUpdate {
        User* user;
        snowflake guild_id;
    };
    struct GuildBanAdd : GuildBanUpdate {};
    struct GuildBanRm : GuildBanUpdate {};
    struct GuildEmojiUpdate {
        snowflake guild_id;
        std::vector<Emoji>* emojis;
    };
    struct GuildIntegrationsUpdate {
        snowflake guild_id;
    };

    struct StatusEvent {
        unsigned long idle_since;
        char* game;
    };

    struct MessageEvent {
        
    };
    
    struct BaseEventHandler {
        void onStatusUpdate();
        void onPresenceUpdate();
        void onMessageCreate(MessageEvent e);
        void onMessageUpdate(MessageEvent e);
    };
}

#endif