#pragma once

//
// Created by connor on 11/18/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>
#include <Game/Global.h>
#include <Sova/Common/Int.h>

namespace DsprFrontend {
    class MessageSender : public Sova::Refable {
    public:
        MessageSender();
        void sendChatMessage(Ref<Sova::String> chatMsg);
        void sendStartGameMessage();
        void sendUnitOrderMessage(Ref<List<Int>> idList, Ref<Int> orderIndex, Ref<List<Int>> otherNumberList);
    private:
        Global* g = nullptr;

        std::shared_ptr<DsprMessage::CStr> getAuthTokenCstr();

        std::shared_ptr<DsprMessage::CStr> authTokenCstr = nullptr;
    };
}
