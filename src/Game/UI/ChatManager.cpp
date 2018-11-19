//
// Created by connor on 11/15/18.
//

#include <Sova/Common/StringBuilder.h>
#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include <Sova/Common/Int.h>
#include "ChatManager.h"
#include "GraphicsManager.h"
#include "Game/Unit/UnitManager.h"
#include "Network/NetworkManager.h"

namespace DsprFrontend {

    ChatManager::ChatManager() {
        this->preStr = New<String>("Chat: ");
        this->messageList = New<List<String>>();
        this->indexList = New<List<Int>>();
        this->nameList = New<List<String>>();
        this->nameList->Add(New<String>("Player1: "));
        this->nameList->Add(New<String>("Player2: "));
    }

//    void ChatManager::setPlayerName(int playerIndex) {
//        auto sb = New<Sova::StringBuilder>();
//        sb->Append(New<String>("Player"));
//        sb->Append(String::getStringFromNumber(playerIndex));
//        this->playerName = sb->ToString();
//    }

    bool ChatManager::isPlayerTyping() {
        return this->playerIsTyping;
    }

    void ChatManager::step() {
        //auto g = (Global*) InternalApp::getGlobal();
        if (this->messageList->Size()>0)
        {
            this->disappearCounter--;
            if (this->disappearCounter <= 0)
            {
                this->disappearCounter = disappearTimeMax;

                this->messageList->Remove(0);
                this->indexList->Remove(0);
            }
        }

        auto enterPressed = InternalApp::keyPressed(Key::Enter);

        if (enterPressed && !this->pressingEnter)
        {
            this->playerIsTyping = !this->playerIsTyping;
            this->pressingEnter = true;
            if (!this->playerIsTyping && this->playerStr->Length()>0)
            {
                this->sendMessage();
                this->playerStr = Null<String>();
            }
        }

        if(!enterPressed) this->pressingEnter = false;

        if (!this->playerIsTyping)return;

        auto backspacePressed = InternalApp::keyPressed(Key::BackSpace);
        if (backspacePressed) this->backspaceCounter++;
        if (backspacePressed && (!this->pressingBackspace || this->backspaceCounter>30))
        {
            this->pressingBackspace = true;
            if (this->playerStr != nullptr && this->playerStr->Length()>0) {
                Ref<String> newStr = Null<String>();
                newStr = this->playerStr->Substring(0, this->playerStr->Length() - 1);
                this->playerStr = nullptr;
                this->playerStr = newStr;
                return;
            }
        }
        if (this->pressingBackspace && !backspacePressed)
        {
            this->pressingBackspace = false;
            this->backspaceCounter = 0;
        }

        if (InternalApp::anyKeyPressed())
        {
            if (this->strDisplayLength > 134)return;
            auto charCaptured = InternalApp::getKeyPressed();
            if (charCaptured == 0)return;
            auto sb = New<StringBuilder>();
            if (this->playerStr != nullptr) sb->Append(this->playerStr);
            sb->Append(New<String>(charCaptured));
            this->playerStr = sb->ToString();
        }

    }

    void ChatManager::sendMessage() {

        auto g = (Global*) InternalApp::getGlobal();

        g->networkManager->messageSender->sendChatMessage(this->playerStr);

        this->addToMessageLog(g->playersTribeIndex, this->playerStr);
    }

    void ChatManager::receiveMessage(int tribeIndex, Ref<String> chatMsg) {
        this->addToMessageLog(tribeIndex, chatMsg);
    }

    void ChatManager::addToMessageLog(int tribeIndex, Ref<String> chatMsg) {
        this->messageList->Add(chatMsg);
        this->indexList->Add(New<Int>(tribeIndex));
    }

    void ChatManager::Draw(Ref<Camera> camera, int xoffset, int yoffset)
    {
        auto g = (Global*) InternalApp::getGlobal();

        if (this->messageList->Size()>0)
        {
            auto upY = 100 - this->messageList->Size()*6;
            auto indexIterator = this->indexList->GetIterator();
            for (auto msgIterator = this->messageList->GetIterator(); msgIterator->Valid(); msgIterator->Next())
            {
                auto message = msgIterator->Get();
                auto index = indexIterator->Get()->getInt();

                g->graphicsManager->drawText(camera, 1, upY, this->nameList->At(index), g->unitManager->getColorFromTribeIndex(index), true);
                g->graphicsManager->drawText(camera, 35, upY, message, Color::White, true);

                upY += 6;
                indexIterator->Next();
            }
        }

        if (!this->playerIsTyping)return;

        g->graphicsManager->drawText(camera, 48, 104, this->preStr, Color::LightGray, true);
        if (this->playerStr != nullptr)
            this->strDisplayLength = g->graphicsManager->drawText(camera, 69, 104, this->playerStr, Color::White, true);
    }
}