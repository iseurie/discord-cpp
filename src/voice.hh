#ifndef H_DSCPP_VOICE
#define H_DSCPP_VOICE

#include "api.hh"

namespace dsc {
    struct VoiceChannel : Pushable {

    }

    struct VoiceState {
        enum SpeakerStatus : char {
            SELF_MUTE   = 0x01,
            SELF_DEAF   = 0x01 >> 1,
            MUTE        = 0x01 >> 2,
            DEAF        = 0x01 >> 3,
            SUPPRESS    = 0x01 >> 4,
        };
        SpeakerStatus stat;
        snowflake guild_id, channel_id, user_id;
        snowflake session_id[2]; // bit-width 128
    }
}

#endif