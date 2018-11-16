#pragma once

//
// Created by connor on 11/15/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/Container.h>

namespace DsprFrontend {
    class ChatManager : public Sova::Container {
    public:
        ChatManager();
        void enablePlayerTyping();
        void Draw(Ref<Camera> camera, int xoffset, int yoffset) override;
        bool isPlayerTyping();
        void step();
        void receiveMessage(int tribeIndex, Ref<String> chatMsg);

    private:

        bool playerIsTyping = false;
        Ref<String> playerStr = Null<String>();
        Ref<String> preStr = Null<String>();

        bool pressingEnter = false;
        bool pressingBackspace = false;
        int backspaceCounter = 0;
        int strDisplayLength = 0;
        Ref<List<String>> messageList = Null<List<String>>();
        Ref<List<Int>> indexList = Null<List<Int>>();
        Ref<List<String>> nameList = Null<List<String>>();

        const int disappearTimeMax = 240;
        int disappearCounter = disappearTimeMax;

        void sendMessage();

        void addToMessageLog(int tribeIndex, Ref<String> chatMsg);
    };
}