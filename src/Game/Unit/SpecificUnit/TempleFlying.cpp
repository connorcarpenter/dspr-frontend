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
        if (upAnimation) {
            if (imageSpeed != 0) {
                auto g = (Global *) InternalApp::getGlobal();
                this->masterUnit->useAnimatedSequenceInfo(g->spriteCatalog->templeGroundToFlying);
                this->masterUnit->imageIndex += imageSpeed;
                if (this->masterUnit->imageIndex >= this->masterUnit->imageNumber) {
                    this->masterUnit->imageIndex = this->masterUnit->imageNumber - 1;
                    this->imageSpeed = 0;
                    this->masterUnit->useAnimatedSequenceInfo(g->spriteCatalog->templeFlying);
                    hoverBuildup = g->spriteCatalog->templeFlying->newAnchorY - g->spriteCatalog->templeBase->anchorY;
                    this->masterUnit->anchor->y = g->spriteCatalog->templeBase->anchorY;
                    toggle = true;
                }
            } else {
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
                } else {
                    if (hoverStep < 1) {
                        hoverStep++;
                    } else {
                        hoverStep = 0;
                        if (hoverBuildup > 0) {

                            if (hoverBuildup < Math::Abs(this->masterUnit->unitTemplate->zBase))
                            {
                                toggle = !toggle;
                                if (!toggle)
                                    this->masterUnit->z += Math::Sign(this->masterUnit->unitTemplate->zBase);
                            }

                            if (toggle) {
                                hoverBuildup -= 1;
                                this->masterUnit->anchor->y += 1;
                                if (hoverBuildup <= 0) {
                                    this->masterUnit->z = this->masterUnit->unitTemplate->zBase;
                                    hoverBuildup = 1;
                                    hovering = true;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            if (imageSpeed != 0) {
                auto g = (Global *) InternalApp::getGlobal();
                this->masterUnit->useAnimatedSequenceInfo(g->spriteCatalog->templeGroundToFlying);
                this->masterUnit->imageIndex += imageSpeed;
                if (this->masterUnit->imageIndex <= 0) {
                    this->masterUnit->imageIndex = 0;
                    this->imageSpeed = 0;
                }
            }
            else {
                if (hoverStep < 1) {
                    hoverStep++;
                } else {
                    hoverStep = 0;
                    if (hoverBuildup > 0) {

                        if (hoverBuildup < Math::Abs(this->masterUnit->unitTemplate->zBase)) {
                            toggle = !toggle;
                            if (!toggle)
                                this->masterUnit->z -= Math::Sign(this->masterUnit->unitTemplate->zBase);
                        }

                        if (toggle) {
                            hoverBuildup -= 1;
                            this->masterUnit->anchor->y -= 1;
                            if (hoverBuildup <= 0) {
                                this->masterUnit->z = 0;
                                imageSpeed = -0.1f;
                                auto g = (Global *) InternalApp::getGlobal();
                                this->masterUnit->useAnimatedSequenceInfo(g->spriteCatalog->templeGroundToFlying);
                                this->masterUnit->imageIndex = this->masterUnit->imageNumber;
                            }
                        }
                    }
                }
            }
        }
    }

    void TempleFlying::specialAction(int actionIndex) {
        switch(actionIndex)
        {
            case 0:
            {
                //Landing action initiation (still need to move into position)
            }
                break;
            case 1:
            {
                //Landing animation begin
                upAnimation = false;
                auto g = (Global *) InternalApp::getGlobal();
                hoverBuildup = g->spriteCatalog->templeFlying->newAnchorY - g->spriteCatalog->templeBase->anchorY + 2;
                hoverStep = 0;
                this->masterUnit->z = this->masterUnit->unitTemplate->zBase;
            }
                break;
        }
    }
}
