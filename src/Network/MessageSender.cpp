//
// Created by connor on 11/18/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Common/StringBuilder.h>
#include "../DsprMessage/ToServerMsg.h"
#include "MessageSender.h"

namespace DsprFrontend {

    MessageSender::MessageSender() {
        this->g = (Global*) InternalApp::getGlobal();
    }

    std::shared_ptr<DsprMessage::CStr> MessageSender::getAuthTokenCstr() {
        if (this->authTokenCstr == nullptr)
        {
            this->authTokenCstr = DsprMessage::CStr::make_cstr((unsigned char*) g->gameServerPlayerToken->AsCStr(), g->gameServerPlayerToken->Length());
        }

        return this->authTokenCstr;
    }

    void MessageSender::sendStandardMessage(Ref<String> str) {

        DsprMessage::ToServerMsg toServerMsg;
        toServerMsg.msgType.set((unsigned char) DsprMessage::ToServerMsg::MessageType::StandardMessage);
        std::shared_ptr<DsprMessage::CStr> cstr = DsprMessage::CStr::make_cstr(str->AsCStr(), str->Length());
        toServerMsg.msgBytes.loadFromCstr(cstr);

        auto packedMsg = toServerMsg.Pack();
        g->gameServer->send(New<String>(packedMsg->getCharPtr(), packedMsg->size(), true));
    }

    void MessageSender::sendStartGameMessage() {
            auto sb = New<Sova::StringBuilder>();
            sb->Append(New<Sova::String>("auth/1.0/gametoken|"));
            sb->Append(g->gameServerPlayerToken);
        this->sendStandardMessage(sb->ToString());
    }

    void MessageSender::sendChatMessage(Ref<Sova::String> chatMsg)
    {
        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("chat/1.0/send|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        sb->Append(chatMsg);
        auto msgToSend = sb->ToString();
        this->sendStandardMessage(sb->ToString());
    }

    void MessageSender::sendUnitOrderMessage(Ref<List<Int>> idList, Ref<Int> orderIndex, Ref<List<Int>> otherNumberList) {
        DsprMessage::UnitOrderMsgV1 unitOrderMsgV1;
        for (auto iterator = idList->GetIterator(); iterator->Valid(); iterator->Next()) {
            auto id = iterator->Get();
            unitOrderMsgV1.unitIds.add(id->getInt());
        }

        unitOrderMsgV1.orderIndex.set(orderIndex->getInt());

        for (auto iterator = otherNumberList->GetIterator(); iterator->Valid(); iterator->Next()) {
            auto id = iterator->Get();
            unitOrderMsgV1.otherNumbers.add(id->getInt());
        }

        auto serverMsg = unitOrderMsgV1.getToServerMessage();
        serverMsg->authToken.loadFromCstr(this->getAuthTokenCstr());
        auto packedMsg = serverMsg->Pack();
        g->gameServer->send(New<String>(packedMsg->getCharPtr(), packedMsg->size(), true));
    }
}