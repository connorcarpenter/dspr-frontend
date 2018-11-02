//
// Created by connor on 8/2/18.
//

#include <Sova/Common/StringBuilder.h>
#include <Sova/Common/String.h>
#include <Sova/Network/HttpRequest.h>
#include <Sova/Internal/InternalApp.h>
#include "NetworkManager.h"
#include "Global.h"
#include <iostream>
#include "TileManager.h"
#include "Unit/UnitManager.h"
#include "EconomyManager.h"

using Sova::String;
using Sova::StringBuilder;

namespace DsprFrontend
{
    NetworkManager::NetworkManager()
    {
        this->g = (Global*) InternalApp::getSovaApp()->getGlobal();

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

        Ref<List<Sova::String>> splitString = message->Split('|');

        if (splitString->Size() != 3) return;

        Ref<Sova::String> command = splitString->At(0);
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

        Ref<List<Sova::String>> splitString = message->Split('|');

        if (splitString->Size() < 2) return;

        Ref<Sova::String> command = splitString->At(0);

        if (command->Equals("auth/1.0/gametoken"))
        {
            auto sb = New<Sova::StringBuilder>();
            sb->Append(New<Sova::String>("auth/1.0/gametoken|"));
            sb->Append(g->gameServerPlayerToken);
            g->gameServer->send(sb->ToString());
            return;
        }
        else if (command->Equals("tribe/1.0/set")) {
            g->playersTribeIndex = atoi(splitString->At(1)->AsCStr());
            return;
        }
        else if (command->Equals("grid/1.0/create")) {
            Ref<List<Sova::String>> gridString = splitString->At(1)->Split(',');
            g->tileManager->receiveGrid(gridString->At(0), gridString->At(1));
            return;
        }
        else if (command->Equals("tile/1.0/create")) {
            Ref<List<Sova::String>> tileString = splitString->At(1)->Split(',');
            g->tileManager->receiveTile(tileString->At(0), tileString->At(1), tileString->At(2));
            return;
        }
        else if (command->Equals("unit/1.0/create")) {
            Ref<List<Sova::String>> unitString = splitString->At(1)->Split(',');
            g->unitManager->receiveUnit(unitString->At(0), unitString->At(1), unitString->At(2), unitString->At(3),
                                        unitString->At(4));
            return;
        }
        else if (command->Equals("unit/1.0/update")) {
            auto idString = splitString->At(1);
            auto propsString = splitString->At(2);
            g->unitManager->receiveUnitUpdate(idString, propsString->Split('&'));
            return;
        }
        else if (command->Equals("unit/1.0/delete")) {
            auto idString = splitString->At(1);
            auto propsString = splitString->At(2);
            g->unitManager->receiveUnitDelete(idString, propsString);
            return;
        }
        else if (command->Equals("economy/1.0/update")) {
            auto propsString = splitString->At(1);
            g->economyManager->receiveUpdate(propsString->Split('&'));
            return;
        }
    }
}
