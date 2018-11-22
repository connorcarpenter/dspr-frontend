//
// Created by connor on 8/2/18.
//

#include <Sova/Common/StringBuilder.h>
#include <Sova/Network/HttpRequest.h>
#include <Sova/Internal/InternalApp.h>
#include "NetworkManager.h"
#include <iostream>
#include <DsprMessage/Main.h>
#include "Game/TileManager.h"
#include "Game/Unit/UnitManager.h"
#include "Game/Item/ItemManager.h"
#include "Game/UI/ChatManager.h"

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
            std::cout << "received bff message!";
            auto addressBody = splitString->At(1);

            Ref<Sova::StringBuilder> sb = New<Sova::StringBuilder>();
            sb->Append(New<Sova::String>("ws://"));
            sb->Append(addressBody);
            Ref<Sova::String> fullAddress = sb->ToString();

            g->gameServerPlayerToken = splitString->At(2);

            g->gameServer = g->app->openWebsocket(fullAddress);
            g->gameServer->setEscapeCharacter(DsprMessage::EscapeCharacter);
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
        //std::cout << "Receive message" << message->AsCStr() << std::endl;
        //message = message->TrimEnd(DsprMessage::EscapeCharacter);

        std::shared_ptr<DsprMessage::CStr> theCStr = DsprMessage::CStr::make_cstr((unsigned char*) message->AsCStr(), message->Length());
        DsprMessage::ToClientMsg clientMsg(theCStr);
        switch (clientMsg.msgType.get())
        {
            case DsprMessage::ToClientMsg::MessageType::UnitUpdate:
            {
                std::shared_ptr<DsprMessage::CStr> unitUpdateCStr = DsprMessage::CStr::make_cstr(clientMsg.msgBytes);
                auto unitUpdateMsg = DsprMessage::UnitUpdateMsgV1(unitUpdateCStr);
                //int id = unitUpdateMsg.id.get();
                g->unitManager->receiveUnitUpdate(unitUpdateMsg);
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::StandardMessage:
            {
                char* newCstr = new char[clientMsg.msgBytes.size()+1];
                for(int i =0;i<clientMsg.msgBytes.size();i++)
                    newCstr[i] = clientMsg.msgBytes.get(i);
                newCstr[clientMsg.msgBytes.size()] = '\0';

                auto newMsg = New<String>(newCstr, true);
                delete [] newCstr;

                auto splitString = newMsg->Split('|');

                if (splitString->At(0)->Equals("auth/1.0/gametoken")) {
                    this->messageSender->sendStartGameMessage();
                    return;
                } else if (splitString->At(0)->Equals("tribe/1.0/set")) {

                    g->playersTribeIndex = atoi(splitString->At(1)->AsCStr());
                    return;
                } else if (splitString->At(0)->Equals("grid/1.0/create")) {

                    auto gridString = splitString->At(1)->Split(',');
                    auto gridWidth = atoi(gridString->At(0)->AsCStr());
                    auto gridHeight = atoi(gridString->At(1)->AsCStr());
                    g->tileManager->receiveGrid(gridWidth, gridHeight);
                    return;
                } else if (splitString->At(0)->Equals("tile/1.0/create")) {

                    auto tileString = splitString->At(1)->Split(',');
                    int tileX = atoi(tileString->At(0)->AsCStr());
                    int tileY = atoi(tileString->At(1)->AsCStr());
                    int tileFrame = atoi(tileString->At(2)->AsCStr());
                    g->tileManager->receiveTile(tileX, tileY, tileFrame);
                    return;
                } else if (splitString->At(0)->Equals("unit/1.0/create")) {

                    auto unitString = splitString->At(1)->Split(',');
                    int id = atoi(unitString->At(0)->AsCStr());
                    int x = atoi(unitString->At(1)->AsCStr());
                    int y = atoi(unitString->At(2)->AsCStr());
                    int tribeIndex = atoi(unitString->At(3)->AsCStr());
                    int templateIndex = atoi(unitString->At(4)->AsCStr());
                    g->unitManager->receiveUnit(id, x, y, tribeIndex, templateIndex);
                    return;
                } else if (splitString->At(0)->Equals("unit/1.0/delete")) {

                    auto idString = splitString->At(1);
                    auto propsString = splitString->At(2);
                    int id = atoi(idString->AsCStr());
                    g->unitManager->receiveUnitDelete(id, propsString);
                    return;
                } else if (splitString->At(0)->Equals("economy/1.0/update")) {
                    //ss->Advance(19);
                    //->economyManager->receiveUpdate(ss);
                    return;
                } else if (splitString->At(0)->Equals("item/1.0/create")) {

                    auto itemString = splitString->At(1)->Split(',');
                    int id = atoi(itemString->At(0)->AsCStr());
                    int x = atoi(itemString->At(1)->AsCStr());
                    int y = atoi(itemString->At(2)->AsCStr());
                    int index = atoi(itemString->At(3)->AsCStr());
                    g->itemManager->receiveItem(id, x, y, index);
                    return;
                } else if (splitString->At(0)->Equals("item/1.0/delete")) {

                    auto idString = splitString->At(1);
                    int id = atoi(idString->AsCStr());
                    g->itemManager->receiveItemDelete(id);
                    return;
                } else if (splitString->At(0)->Equals("chat/1.0/send")) {

                    auto tribeIndex = atoi(splitString->At(1)->AsCStr());
                    auto msgString = splitString->At(2);
                    g->chatManager->receiveMessage(tribeIndex, msgString);
                    return;
                }
            }
            break;
            default:
                int i = 12; //????
                break;
        }
    }
}
