//
// Created by connor on 11/15/18.
//

#include <Sova/Common/StringBuilder.h>
#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "ChatManager.h"
#include "GraphicsManager.h"

namespace DsprFrontend {

    ChatManager::ChatManager() {
        this->preStr = New<String>("Chat: ");
    }

//    void ChatManager::setPlayerName(int playerIndex) {
//        auto sb = New<Sova::StringBuilder>();
//        sb->Append(New<String>("Player"));
//        sb->Append(String::getStringFromNumber(playerIndex));
//        this->playerName = sb->ToString();
//    }

    void ChatManager::Draw(Ref<Camera> camera, int xoffset, int yoffset) {
        if (!this->playerIsTyping)return;
        auto g = (Global*) InternalApp::getGlobal();
        g->graphicsManager->drawText(camera, 48, 104, this->preStr, Color::LightGray, true);
        if (this->playerStr != nullptr)
            this->strDisplayLength = g->graphicsManager->drawText(camera, 69, 104, this->playerStr, Color::White, true);
    }

    bool ChatManager::isPlayerTyping() {
        return this->playerIsTyping;
    }

    void ChatManager::step() {
        //auto g = (Global*) InternalApp::getGlobal();

        auto enterPressed = InternalApp::keyPressed(Key::Enter);

        if (enterPressed && !this->pressingEnter)
        {
            this->playerIsTyping = !this->playerIsTyping;
            this->pressingEnter = true;
            if (!this->playerIsTyping && this->playerStr->Length()>0)
            {
                this->SendPlayerStr();
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

    void ChatManager::SendPlayerStr() {

        auto g = (Global*) InternalApp::getGlobal();

        auto sb = New<Sova::StringBuilder>();
        sb->Append(New<Sova::String>("chat/1.0/send|"));
        sb->Append(g->gameServerPlayerToken);
        sb->Append(New<Sova::String>("|"));
        sb->Append(this->playerStr);
        g->gameServer->send(sb->ToString());
    }

    void ChatManager::receiveMessage(int tribeIndex, Ref<String> chatMsg) {

    }
}