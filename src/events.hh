#ifndef H_DSCPP_EVENTS
#define H_DSCPP_EVENTS

namespace dsc {
    struct BaseEventHandler {
        void onPresenceUpdate();
        void onMessageCreate(MessageEvent e);
        void onMessageUpdate(MessageEvent e);
    }

    struct MessageEvent {
        
    }
}

#endif