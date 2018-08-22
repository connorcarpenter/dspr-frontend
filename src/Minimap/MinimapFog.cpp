//
// Created by connor on 8/20/18.
//

#include <Sova/Internal/InternalApp.h>
#include <Global.h>
#include "MinimapFog.h"
#include "FogManager.h"

namespace DsprFrontend {

    MinimapFog::MinimapFog()
    {
        this->g = (Global*) InternalApp::getSovaApp()->getGlobal();
    }

    void MinimapFog::drawSelf(Sova::Ref<Camera> camera, int xoffset, int yoffset)
    {
        if (this->gridW == -1)
        {
            auto gridSize = g->tileManager->getGridSize();
            if (gridSize->x != 0)
            {
                this->cellW = gridSize->x / 4;
                this->cellH = gridSize->y / 4;
                this->gridW = gridSize->x;
                this->gridH = gridSize->y;
            }
            return;
        }

        this->cellX += this->cellW;
        if (this->cellX >= this->gridW) {
            this->cellX = 0;
            this->cellY += this->cellH;
            if (this->cellY >= this->gridH) {
                this->cellX = 0;
                this->cellY = 0;
            }
        }
        this->g->fogManager->minimapDrawFog(camera, this->cellX, this->cellY, this->cellW, this->cellH);
    }
}