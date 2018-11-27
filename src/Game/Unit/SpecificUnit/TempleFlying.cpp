//
// Created by connor on 10/28/18.
//

#include <Sova/Math/Math.h>
#include <Sova/Internal/InternalApp.h>
#include <Game/Global.h>
#include "TempleFlying.h"
#include "Game/Unit/Unit.h"
#include "SpriteCatalog.h"

namespace DsprFrontend
{
    TempleFlying::TempleFlying(Unit* masterUnit) {
        this->masterUnit = masterUnit;

        auto g = (Global*) InternalApp::getGlobal();
        this->masterUnit->useAnimatedSequenceInfo(g->spriteCatalog->templeGroundToFlying);
        this->imageSpeed = 0.1f;
        this->masterUnit->imageIndex = 0;
        this->masterUnit->z = 0;
    }

    void TempleFlying::stepFunction() {
        //special animation
        if (imageSpeed != 0)
        {
            auto g = (Global*) InternalApp::getGlobal();
            this->masterUnit->useAnimatedSequenceInfo(g->spriteCatalog->templeGroundToFlying);
            this->masterUnit->imageIndex += imageSpeed;
            if (this->masterUnit->imageIndex >= this->masterUnit->imageNumber)
            {
                this->masterUnit->imageIndex = this->masterUnit->imageNumber-1;
                this->imageSpeed = 0;
                this->masterUnit->useAnimatedSequenceInfo(g->spriteCatalog->templeFlying);
                hoverBuildup = g->spriteCatalog->templeFlying->newAnchorY - g->spriteCatalog->templeBase->anchorY;
                this->masterUnit->anchor->y = g->spriteCatalog->templeBase->anchorY;
            }
        }
        else {
            if (hovering) {
                //hovering
                if (hoverStep < 40) {
                    hoverStep++;
                } else {
                    hoverStep = 0;
                    if (hoverBuildup < 2) {
                        this->masterUnit->z += (hoverUp) ? -1 : 1;
                        hoverBuildup += 1;
                    } else {
                        hoverUp = !hoverUp;
                        hoverBuildup = 0;
                        hoverStep = -40;
                    }
                }
            }
            else
            {
                if (hoverStep < 1) {
                    hoverStep++;
                }
                else {
                    hoverStep=0;
                    if (hoverBuildup > 0) {
                        hoverBuildup -= 1;
                        if (hoverBuildup < Math::Abs(this->masterUnit->unitTemplate->zBase))
                        {
                            this->masterUnit->z += Math::Sign(this->masterUnit->unitTemplate->zBase);
                        }
                        this->masterUnit->anchor->y += 1;
                        if (hoverBuildup <= 0)
                        {
                            this->masterUnit->z = this->masterUnit->unitTemplate->zBase;
                            hoverBuildup = 1;
                            hovering = true;
                        }
                    }
                }
            }
        }
    }

    void TempleFlying::specialAction(int actionIndex) {
        int i = 0;
    }
}
