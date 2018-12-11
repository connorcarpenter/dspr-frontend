#pragma once

//
// Created by connor on 11/18/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>

namespace DsprFrontend {
    class MessageReceiver : public Sova::Refable {
    public:
        void receiveMessage(Ref<Sova::String> message);
    };
}
