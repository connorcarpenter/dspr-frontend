//
// Created by connor on 11/18/18.
//

#include <DsprMessage/CStr.h>
#include <DsprMessage/ToClientMsg.h>
#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include <Game/Effects/Projectile.h>
#include "MessageReceiver.h"
#include "Game/Unit/UnitManager.h"
#include "Game/TileManager.h"
#include "Game/EconomyManager.h"
#include "Game/Item/ItemManager.h"
#include "Game/UI/ChatManager.h"
#include "NetworkManager.h"
#include "Game/TribeManager.h"

namespace DsprFrontend
{
    void MessageReceiver::receiveMessage(Ref<Sova::String> message)
    {
        //std::cout << "Receive message" << message->AsCStr() << std::endl;
        //message = message->TrimEnd(DsprMessage::EscapeCharacter);

        auto g = (Global*) InternalApp::getGlobal();

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
                g->tribeManager->setTribeStatus(tribeSetMsg.tribeIndex.get(), tribeSetMsg.status.get());
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
            case DsprMessage::ToClientMsg::MessageType::UnitSpecialAction:
            {
                auto unitSpecialActionMsg = DsprMessage::UnitSpecialActionMsgV1(clientMsg.msgBytes);
                g->unitManager->receiveUnitSpecialAction(unitSpecialActionMsg.id.get(),
                                                         unitSpecialActionMsg.actionIndex.get());
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
                g->networkManager->messageSender->sendStartGameMessage();
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