//
// Created by connor on 11/18/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Sova/Common/StringBuilder.h>
#include "../DsprMessage/ToServerMsg.h"
#include "MessageSender.h"
#include "../DsprMessage/Array.h"

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
        std::shared_ptr<DsprMessage::CStr> cstr = DsprMessage::CStr::make_cstr((const unsigned char*) str->AsCStr(), str->Length());
        toServerMsg.msgBytes.loadFromCstr(cstr);

        auto packedMsg = toServerMsg.Pack();
        g->gameServer->send(New<String>((char*) packedMsg->getCharPtr(), packedMsg->size(), true));
    }

    void MessageSender::sendStartGameMessage() {
            auto sb = New<Sova::StringBuilder>();
            sb->Append(New<Sova::String>("auth/1.0/gametoken|"));
            sb->Append(g->gameServerPlayerToken);
        this->sendStandardMessage(sb->ToString());
    }

    void MessageSender::sendChatMessage(Ref<Sova::String> chatMsg)
    {
        DsprMessage::ChatSendServerMsgV1 chatSendServerMsgV1;
        chatSendServerMsgV1.chatMsg.loadFromCharPtr(chatMsg->AsCStr(), chatMsg->Length());
        auto serverMsg = chatSendServerMsgV1.getToServerMessage();
        serverMsg->authToken.loadFromCstr(this->getAuthTokenCstr());
        auto packedMsg = serverMsg->Pack();
        g->gameServer->send(New<String>((char*) packedMsg->getCharPtr(), packedMsg->size(), true));
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

//        //TESTING
//        std::shared_ptr<DsprMessage::CStr> copiedPackedMsg = packedMsg->getCopy();
//        DsprMessage::ToServerMsg testServerMsg = DsprMessage::ToServerMsg(copiedPackedMsg);
//        DsprMessage::UnitOrderMsgV1 testUnitOrderMsg = DsprMessage::UnitOrderMsgV1(testServerMsg.msgBytes);
//        std::shared_ptr<DsprMessage::CStr> testCstr = DsprMessage::CStr::make_cstr(testServerMsg.msgBytes);
//        DsprMessage::CStr* testCstrR = testCstr.get();
//        auto testAuthToken = serverMsg->authToken.toStdString();
//        std::string testATR = *testAuthToken.get();
//        //TESTING

        g->gameServer->send(New<String>((char*) packedMsg->getCharPtr(), packedMsg->size(), true));
    }
}