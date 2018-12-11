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

using Sova::String;
using Sova::StringBuilder;

namespace DsprFrontend
{
    NetworkManager::NetworkManager()
    {
        this->g = (Global*) InternalApp::getGlobal();
        this->messageSender = New<MessageSender>();
        this->messageReceiver = New<MessageReceiver>();

        //Ref<HttpRequest> bffReq = g->app->makeHttpRequest(New<Sova::String>("GET"), New<Sova::String>("http://www.deusporadev.com:3170/orchestrator/bff"));
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
        this->messageReceiver->receiveMessage(message);
    }
}
