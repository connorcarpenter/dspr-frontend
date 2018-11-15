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

    int GraphicsManager::drawText(Ref<Camera> camera, int x, int y, Ref<String> str, Color color, bool alignLeft) {
        this->myFont->position->set(x, y);
        this->myFont->tint = color;

        if (alignLeft)
        {
            int charIndex = 0;
            auto charStr = str->AsCStr();
            while(charStr[charIndex] != '\0')
            {
                this->myFont->imageIndex = charStr[charIndex]-33;
                if (this->myFont->imageIndex < 0 || this->myFont->imageIndex > 126) {
                    this->myFont->position->x += 3;
                    charIndex++;
                    continue;
                }
                this->myFont->drawSelf(camera, 0, 0);
                this->myFont->position->x+= letterWidth((int) this->myFont->imageIndex)+1;
                charIndex++;
            }
            return this->myFont->position->x - x;
        }
        else
        {
            int charIndex = 0;
            auto charStr = str->AsCStr();
            while(charStr[charIndex] != '\0') charIndex++;
            charIndex--;
            while(charIndex>=0)
            {
                this->myFont->imageIndex = charStr[charIndex]-33;
                this->myFont->drawSelf(camera, 0, 0);
                this->myFont->position->x-=6;
                charIndex--;
            }
        }
    }

    void GraphicsManager::drawChar(Ref<Camera> camera, int x, int y, char c, Color color) {
        this->myFont->position->set(x, y);
        this->myFont->tint = color;
        this->myFont->imageIndex = c-33;
        this->myFont->drawSelf(camera, 0, 0);
        this->myFont->position->x-=4;
    }

    int NumDigits(int x)
    {
        x = abs(x);
        return (x < 10 ? 1 :
                (x < 100 ? 2 :
                 (x < 1000 ? 3 :
                  (x < 10000 ? 4 :
                   (x < 100000 ? 5 :
                    (x < 1000000 ? 6 :
                     (x < 10000000 ? 7 :
                      (x < 100000000 ? 8 :
                       (x < 1000000000 ? 9 :
                        10)))))))));
    }

    void GraphicsManager::drawNumber(Ref<Camera> camera, int x, int y, int number, Color color, bool alignLeft) {
        this->myFont->position->set(x, y);
        this->myFont->tint = color;

        if (alignLeft)
        {
            int digits = NumDigits(number);
            this->myFont->position->x+=4*digits;
        }

        while(true)
        {
            auto digit = number % 10;
            this->myFont->imageIndex = digit+15;
            this->myFont->drawSelf(camera, 0, 0);
            this->myFont->position->x-=4;

            if (number < 10)break;
            number /= 10;
        }
    }
}