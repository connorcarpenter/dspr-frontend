//
// Created by connor on 11/18/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Common/StringBuilder.h>
#include "MessageSender.h"

namespace DsprFrontend {

    MessageSender::MessageSender() {
        this->g = (Global*) InternalApp::getGlobal();
    }

    void MessageSender::sendStartGameMessage() {
            auto sb = New<Sova::StringBuilder>();
            sb->Append(New<Sova::String>("auth/1.0/gametoken|"));
            sb->Append(g->gameServerPlayerToken);
            g->gameServer->send(sb->ToString());
    }

    void MessageSender::sendChatMessage(Ref<Sova::String> chatMsg)
    {
        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("chat/1.0/send|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        sb->Append(chatMsg);
        auto msgToSend = sb->ToString();
        g->gameServer->send(msgToSend);
    }


}