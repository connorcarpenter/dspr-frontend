#pragma once

//
// Created by connor on 11/18/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>
#include <Game/Global.h>

namespace DsprFrontend {
    class MessageSender : public Sova::Refable {
    public:
        MessageSender();
        void sendChatMessage(Ref<Sova::String> chatMsg);
        void sendStartGameMessage();
    private:
        Global* g = nullptr;
    };
}
