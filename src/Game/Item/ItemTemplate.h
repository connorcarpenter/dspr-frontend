#pragma once

//
// Created by connor on 11/13/18.
//

#include <Sova/References/Refable.h>

namespace DsprFrontend {

    enum BodyPart { None, Any, Head, Body, Hand };

    class ItemTemplate : public Sova::Refable {
    public:
        ItemTemplate(int index);
        int index;
        BodyPart wornOn = BodyPart::None;
    };
}