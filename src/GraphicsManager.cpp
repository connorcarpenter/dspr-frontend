//
// Created by connor on 10/31/18.
//

#include <Sova/Graphics/Camera.h>
#include "GraphicsManager.h"

namespace DsprFrontend
{
    GraphicsManager::GraphicsManager()
    {
        this->myFont = New<AnimatedSprite>(New<Sova::String>("images/ui/myFont.png"), 7, 7, 1);
    }

    void GraphicsManager::drawText(Ref<Camera> camera, int x, int y, Ref<String> str, Color color, bool alignLeft) {
        this->myFont->position->set(x, y);
        this->myFont->tint = color;
        int charIndex = 0;
        auto charStr = str->AsCStr();
        while(charStr[charIndex] != '\0') charIndex++;
        charIndex--;
        while(charIndex>=0)
        {
            this->myFont->imageIndex = charStr[charIndex]-33;
            this->myFont->drawSelf(camera, 0, 0);
            this->myFont->position->x-=4;
            charIndex--;
        }
    }
}