//
// Created by connor on 8/2/18.
//

#include <Sova/Common/StringBuilder.h>
#include <Sova/Network/HttpRequest.h>
#include <Sova/Internal/InternalApp.h>
#include "NetworkManager.h"
#include <iostream>
#include <DsprMessage/Main.h>
#include <Game/Effects/Projectile.h>
#include "Game/TileManager.h"
#include "Game/Unit/UnitManager.h"
#include "Game/Item/ItemManager.h"
#include "Game/UI/ChatManager.h"
#include "Game/EconomyManager.h"

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
                auto unitUpdateMsg = DsprMessage::UnitUpdateMsgV1(clientMsg.msgBytes);
                g->unitManager->receiveUnitUpdate(unitUpdateMsg);
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::TribeSet:
            {
                auto tribeSetMsg = DsprMessage::TribeSetMsgV1(clientMsg.msgBytes);
                g->playersTribeIndex = tribeSetMsg.tribeIndex.get();
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::GridCreate:
            {
                auto gridCreateMsg = DsprMessage::GridCreateMsgV1(clientMsg.msgBytes);
                g->tileManager->receiveGrid(gridCreateMsg.width.get(), gridCreateMsg.height.get());
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::TileCreate:
            {
                auto tileCreateMsg = DsprMessage::TileCreateMsgV1(clientMsg.msgBytes);
                g->tileManager->receiveTile(tileCreateMsg.x.get(), tileCreateMsg.y.get(), tileCreateMsg.frame.get());
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::UnitCreate:
            {
                auto unitCreateMsg = DsprMessage::UnitCreateMsgV1(clientMsg.msgBytes);
                g->unitManager->receiveUnit(unitCreateMsg.id.get(),
                                            unitCreateMsg.x.get(),
                                            unitCreateMsg.y.get(),
                                            unitCreateMsg.tribeIndex.get(),
                                            unitCreateMsg.templateIndex.get());
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::UnitDelete:
            {
                auto unitDeleteMsgV1 = DsprMessage::UnitDeleteMsgV1(clientMsg.msgBytes);
                g->unitManager->receiveUnitDelete(unitDeleteMsgV1.id.get(),
                                                  unitDeleteMsgV1.dead.get() == 1);
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::EconomyUpdate:
            {
                auto economyUpdateMsgV1 = DsprMessage::EconomyUpdateMsgV1(clientMsg.msgBytes);
                if (economyUpdateMsgV1.mana.getWasSet())
                    g->economyManager->setMana(economyUpdateMsgV1.mana.get());
                if (economyUpdateMsgV1.pop.getWasSet())
                    g->economyManager->setPop(economyUpdateMsgV1.pop.get());
                if (economyUpdateMsgV1.popMax.getWasSet())
                    g->economyManager->setPopMax(economyUpdateMsgV1.popMax.get());
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::ItemCreate:
            {
                auto itemCreateMsgV1 = DsprMessage::ItemCreateMsgV1(clientMsg.msgBytes);
                g->itemManager->receiveItem(itemCreateMsgV1.id.get(),
                                            itemCreateMsgV1.x.get(),
                                            itemCreateMsgV1.y.get(),
                                            itemCreateMsgV1.templateIndex.get());
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::ItemDelete:
            {
                auto itemDeleteMsgV1 = DsprMessage::ItemDeleteMsgV1(clientMsg.msgBytes);
                g->itemManager->receiveItemDelete(itemDeleteMsgV1.id.get());
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::ChatSend:
            {
                auto chatSendMsgV1 = DsprMessage::ChatSendClientMsgV1(clientMsg.msgBytes);

                char* cstr = new char[chatSendMsgV1.chatMsg.size()];
                for(int i = 0;i<chatSendMsgV1.chatMsg.size();i++)
                    cstr[i] = chatSendMsgV1.chatMsg.get(i);

                Ref<String> chatStr = New<String>(cstr, chatSendMsgV1.chatMsg.size(), true);
                g->chatManager->receiveMessage(chatSendMsgV1.tribeIndex.get(),
                                               chatStr);
                delete [] cstr;
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::AuthGameToken:
            {
                this->messageSender->sendStartGameMessage();
                return;
            }
                break;
            case DsprMessage::ToClientMsg::MessageType::ProjectileCreate:
            {
                auto projectileCreateMsg = DsprMessage::ProjectileCreateMsgV1(clientMsg.msgBytes);
                g->world->AddChild(New<Projectile>(projectileCreateMsg.from.get(0),
                                                   projectileCreateMsg.from.get(1),
                                                   projectileCreateMsg.to.get(0),
                                                   projectileCreateMsg.to.get(1),
                                                   projectileCreateMsg.index.get()));
                return;
            }
                break;
            default:
                int i = 12; //????
                break;
        }
    }
}
