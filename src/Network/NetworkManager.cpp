//
// Created by connor on 8/2/18.
//

#include <Sova/Common/StringBuilder.h>
#include <Sova/Common/String.h>
#include <Sova/Network/HttpRequest.h>
#include <Sova/Internal/InternalApp.h>
#include "NetworkManager.h"
#include "Game/Global.h"
#include <iostream>
#include "Game/TileManager.h"
#include "Game/Unit/UnitManager.h"
#include "Game/EconomyManager.h"
#include "Game/Item/ItemManager.h"
#include "Game/UI/ChatManager.h"
#include "StringScanner.h"
#include "DsprMessage/ToClientMsg.h"
#include "DsprMessage/DataTypes.h"

using Sova::String;
using Sova::StringBuilder;

namespace DsprFrontend
{
    NetworkManager::NetworkManager()
    {
        this->g = (Global*) InternalApp::getGlobal();
        this->messageSender = New<MessageSender>();
        this->messageReceiver = New<MessageReceiver>();

        Ref<HttpRequest> bffReq = g->app->makeHttpRequest(New<Sova::String>("GET"), New<Sova::String>("http://www.deuspora.com:3170/orchestrator/bff"));
        bffReq->onResponse(
                [&](Ref<HttpResponse> response)
                {
                    if (response->status == 200 && response->responseText->Length() > 0)
                    {
                        g->bffServer = g->app->openWebsocket(response->responseText);
                        g->bffServer->onOpen(
                                [&](){
                                    this->OnBffServerStart();
                                });
                        g->bffServer->onMessage(
                                [&](Ref<Sova::String> message){
                                    this->OnBffServerUpdate(message);
                                });
                    }
                });
        bffReq->send();
    }

    void NetworkManager::OnBffServerStart()
    {
        g->bffServer->send(New<Sova::String>("gameservers/1.0/join"));
    }

    void NetworkManager::OnBffServerUpdate(Ref<Sova::String> message)
    {
        message = message->TrimEnd("\r\n");

        auto splitString = message->Split('|');

        if (splitString->Size() != 3) return;

        auto command = splitString->At(0);
        if (command->Equals("gameserver/connect"))
        {
            auto addressBody = splitString->At(1);

            Ref<Sova::StringBuilder> sb = New<Sova::StringBuilder>();
            sb->Append(New<Sova::String>("ws://"));
            sb->Append(addressBody);
            Ref<Sova::String> fullAddress = sb->ToString();

            g->gameServerPlayerToken = splitString->At(2);

            g->gameServer = g->app->openWebsocket(fullAddress);
            g->gameServer->onOpen(
                    [&](){
                        this->OnGameServerStart();
                    });
            g->gameServer->onMessage(
                    [&](Ref<Sova::String> message){
                        this->OnGameServerUpdate(message);
                    });
        }
    }

    void NetworkManager::OnGameServerStart()
    {
        std::cout << "Gameserver Start" << std::endl;
    }

    void NetworkManager::OnGameServerUpdate(Ref<Sova::String> message)
    {
        message = message->TrimEnd("\r\n")->TrimStart("\n");

        DsprMessage::_cstr theCStr((char*) message->AsCStr(), message->Length());
        DsprMessage::ToClientMsg clientMsg(theCStr);
        if (clientMsg.msgType.get() == DsprMessage::ToClientMsg::UnitUpdate)
        {
            DsprMessage::UnitUpdateMsgV1 unitUpdateMsg(clientMsg.msgBytes.get());
            //int id = unitUpdateMsg.id.get();
            g->unitManager->receiveUnitUpdate(unitUpdateMsg);
            return;
        }
        else
        if (clientMsg.msgType.get() == DsprMessage::ToClientMsg::StandardMessage)
        {
            auto msgBytes = clientMsg.msgBytes.get();
            auto newMsg = New<String>(msgBytes.innerCstr, msgBytes.number, true);
            auto ss = New<StringScanner>(newMsg);

            if (ss->EqualsUntil("auth/1.0/gametoken", '|')) {
                this->messageSender->sendStartGameMessage();
                return;
            } else if (ss->EqualsUntil("tribe/1.0/set", '|')) {
                auto splitString = message->Split('|');
                g->playersTribeIndex = atoi(splitString->At(1)->AsCStr());
                return;
            } else if (ss->EqualsUntil("grid/1.0/create", '|')) {
                auto splitString = message->Split('|');
                auto gridString = splitString->At(1)->Split(',');
                auto gridWidth = atoi(gridString->At(0)->AsCStr());
                auto gridHeight = atoi(gridString->At(1)->AsCStr());
                g->tileManager->receiveGrid(gridWidth, gridHeight);
                return;
            } else if (ss->EqualsUntil("tile/1.0/create", '|')) {
                auto splitString = message->Split('|');
                auto tileString = splitString->At(1)->Split(',');
                int tileX = atoi(tileString->At(0)->AsCStr());
                int tileY = atoi(tileString->At(1)->AsCStr());
                int tileFrame = atoi(tileString->At(2)->AsCStr());
                g->tileManager->receiveTile(tileX, tileY, tileFrame);
                return;
            } else if (ss->EqualsUntil("unit/1.0/create", '|')) {
                auto splitString = message->Split('|');
                auto unitString = splitString->At(1)->Split(',');
                int id = atoi(unitString->At(0)->AsCStr());
                int x = atoi(unitString->At(1)->AsCStr());
                int y = atoi(unitString->At(2)->AsCStr());
                int tribeIndex = atoi(unitString->At(3)->AsCStr());
                int templateIndex = atoi(unitString->At(4)->AsCStr());
                g->unitManager->receiveUnit(id, x, y, tribeIndex, templateIndex);
                return;
            } else if (ss->EqualsUntil("unit/1.0/update", '|')) {
                int what = 0;//huh? how'd we get here
                auto splitString = message->Split('|');
                auto idString = splitString->At(1);
                auto propsString = splitString->At(2);
                int id = atoi(idString->AsCStr());
                g->unitManager->receiveUnitUpdate(id, propsString->Split('&'));
                return;
            } else if (ss->EqualsUntil("unit/1.0/delete", '|')) {
                auto splitString = message->Split('|');
                auto idString = splitString->At(1);
                auto propsString = splitString->At(2);
                int id = atoi(idString->AsCStr());
                g->unitManager->receiveUnitDelete(id, propsString);
                return;
            } else if (ss->EqualsUntil("economy/1.0/update", '|')) {
                ss->Advance(19);
                g->economyManager->receiveUpdate(ss);
                return;
            } else if (ss->EqualsUntil("item/1.0/create", '|')) {
                auto splitString = message->Split('|');
                auto itemString = splitString->At(1)->Split(',');
                int id = atoi(itemString->At(0)->AsCStr());
                int x = atoi(itemString->At(1)->AsCStr());
                int y = atoi(itemString->At(2)->AsCStr());
                int index = atoi(itemString->At(3)->AsCStr());
                g->itemManager->receiveItem(id, x, y, index);
                return;
            } else if (ss->EqualsUntil("item/1.0/delete", '|')) {
                auto splitString = message->Split('|');
                auto idString = splitString->At(1);
                int id = atoi(idString->AsCStr());
                g->itemManager->receiveItemDelete(id);
                return;
            } else if (ss->EqualsUntil("chat/1.0/send", '|')) {
                auto splitString = message->Split('|');
                auto tribeIndex = atoi(splitString->At(1)->AsCStr());
                auto msgString = splitString->At(2);
                g->chatManager->receiveMessage(tribeIndex, msgString);
                return;
            }
        }
    }

//    void NetworkManager::OnGameServerUpdate(Ref<Sova::String> message)
//    {
//        message = message->TrimEnd("\r\n")->TrimStart("\n");
//
//        auto splitString = message->Split('|');
//
//        if (splitString->Size() < 2) return;
//
//        auto command = splitString->At(0);
//
//        if (command->Equals("auth/1.0/gametoken"))
//        {
//            this->messageSender->sendStartGameMessage();
//            return;
//        }
//        else if (command->Equals("tribe/1.0/set")) {
//            g->playersTribeIndex = atoi(splitString->At(1)->AsCStr());
//            return;
//        }
//        else if (command->Equals("grid/1.0/create")) {
//            auto gridString = splitString->At(1)->Split(',');
//            auto gridWidth = atoi(gridString->At(0)->AsCStr());
//            auto gridHeight = atoi(gridString->At(1)->AsCStr());
//            g->tileManager->receiveGrid(gridWidth, gridHeight);
//            return;
//        }
//        else if (command->Equals("tile/1.0/create")) {
//            auto tileString = splitString->At(1)->Split(',');
//            int tileX = atoi(tileString->At(0)->AsCStr());
//            int tileY = atoi(tileString->At(1)->AsCStr());
//            int tileFrame = atoi(tileString->At(2)->AsCStr());
//            g->tileManager->receiveTile(tileX, tileY, tileFrame);
//            return;
//        }
//        else if (command->Equals("unit/1.0/create")) {
//            auto unitString = splitString->At(1)->Split(',');
//            int id = atoi(unitString->At(0)->AsCStr());
//            int x = atoi(unitString->At(1)->AsCStr());
//            int y = atoi(unitString->At(2)->AsCStr());
//            int tribeIndex = atoi(unitString->At(3)->AsCStr());
//            int templateIndex = atoi(unitString->At(4)->AsCStr());
//            g->unitManager->receiveUnit(id, x, y, tribeIndex, templateIndex);
//            return;
//        }
//        else if (command->Equals("unit/1.0/update")) {
//            auto idString = splitString->At(1);
//            auto propsString = splitString->At(2);
//            int id = atoi(idString->AsCStr());
//            g->unitManager->receiveUnitUpdate(id, propsString->Split('&'));
//            return;
//        }
//        else if (command->Equals("unit/1.0/delete")) {
//            auto idString = splitString->At(1);
//            auto propsString = splitString->At(2);
//            int id = atoi(idString->AsCStr());
//            g->unitManager->receiveUnitDelete(id, propsString);
//            return;
//        }
//        else if (command->Equals("economy/1.0/update")) {
//            auto propsString = splitString->At(1);
//            g->economyManager->receiveUpdate(propsString->Split('&'));
//            return;
//        }
//        else if (command->Equals("item/1.0/create")) {
//            auto itemString = splitString->At(1)->Split(',');
//            int id = atoi(itemString->At(0)->AsCStr());
//            int x = atoi(itemString->At(1)->AsCStr());
//            int y = atoi(itemString->At(2)->AsCStr());
//            int index = atoi(itemString->At(3)->AsCStr());
//            g->itemManager->receiveItem(id, x, y, index);
//            return;
//        }
//        else if (command->Equals("item/1.0/delete")) {
//            auto idString = splitString->At(1);
//            int id = atoi(idString->AsCStr());
//            g->itemManager->receiveItemDelete(id);
//            return;
//        }
//        else if (command->Equals("chat/1.0/send")) {
//            auto tribeIndex = atoi(splitString->At(1)->AsCStr());
//            auto msgString = splitString->At(2);
//            g->chatManager->receiveMessage(tribeIndex, msgString);
//            return;
//        }
//    }
}
