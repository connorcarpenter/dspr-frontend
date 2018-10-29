//
// Created by connor on 10/28/18.
//

#include <Sova/Math/Math.h>
#include "Manafount.h"
#include "Unit/Unit.h"

namespace DsprFrontend
{
    Manafount::Manafount(Unit* masterUnit) {
        this->masterUnit = masterUnit;

        this->lastAnim = (unsigned short) Sova::Math::Random(0,4);
        this->lastLastAnim = (unsigned short) ((lastAnim + 1) % 4);
    }

    void Manafount::stepFunction() {
        if (wait > 0){
            wait-=1;
        }
        else
        {
            if (this->imageIndex == 0) {
                bool skipOne = Sova::Math::Random(0, 2) > 1;

                for (int i = 0; i < 4; i++) {
                    if (i == this->lastAnim || i == this->lastLastAnim)continue;
                    if (skipOne) {
                        skipOne = false;
                        continue;
                    }
                    this->baseFrame = (i*4) + 1;
                    this->lastLastAnim = this->lastAnim;
                    this->lastAnim = i;
                    break;
                }

                this->imageIndex += 0.01f;

            }
            else
            {
                this->imageIndex += 0.15f;

                if (this->imageIndex >= 4)
                {
                    this->wait = (unsigned short) Sova::Math::Random(1,50);
                    this->imageIndex = 0;
                    this->baseFrame = 0;
                }
            }

            this->masterUnit->imageIndex = this->baseFrame + this->imageIndex;
        }
    }
}
