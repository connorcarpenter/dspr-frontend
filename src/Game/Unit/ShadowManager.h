#pragma once

//
// Created by connor on 10/31/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>
#include <Sova/Graphics/Container.h>

using namespace Sova;

namespace DsprFrontend {
    class ShadowManager : public Sova::Container {
    public:
        ShadowManager();
        void drawSelf(Ref<Camera> camera, int xoffset, int yoffset) override;
    };
}