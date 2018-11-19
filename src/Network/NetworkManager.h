#pragma once

//
// Created by connor on 8/2/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>
#include <Sova/References/Ref.h>
#include "MessageSender.h"
#include "MessageReceiver.h"

namespace DsprFrontend
{
    class Global;

    class NetworkManager : public Sova::Refable
    {
    public:
        NetworkManager();

        Ref<MessageSender> messageSender = Null<MessageSender>();
        Ref<MessageReceiver> messageReceiver = Null<MessageReceiver>();

    private:
        void OnBffServerStart();
        void OnBffServerUpdate(Ref<Sova::String> message);

        void OnGameServerStart();
        void OnGameServerUpdate(Ref<Sova::String> message);

        Global* g = nullptr;
    };
}
