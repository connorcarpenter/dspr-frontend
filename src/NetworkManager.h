#pragma once

//
// Created by connor on 8/2/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>
#include <Sova/References/Ref.h>

namespace DsprFrontend
{
    class Global;

    class NetworkManager : public Sova::Refable
    {
    public:
        NetworkManager();
    private:
        void OnBffServerStart();
        void OnBffServerUpdate(Ref<Sova::String> message);

        void OnGameServerStart();
        void OnGameServerUpdate(Ref<Sova::String> message);

        Global* g = nullptr;
    };
}
