//
// Created by connor on 8/16/18.
//

#include <Sova/Math/Math.h>
#include <Modules/Core/String/String.h>
#include <Sova/Internal/InternalApp.h>
#include "FogManager.h"
#include "Sova/Graphics/Internal/InternalCamera.h"
#include "Global.h"
#include "Game/Minimap/Minimap.h"
#include "Circle/CircleCache.h"

namespace DsprFrontend
{
    FogManager::FogManager() {
        this->minimapPixel = New<Pixel>();
        this->minimapPixel->setLineStyle(1, Color::Gray, 0.75f);

        this->minimapRect = New<Rectangle>(0,0);
        this->minimapRect->setLineStyle(false);
        this->minimapRect->setFillStyle(Color::Black, 0.0f);
        this->minimapRect->position->x = -1;
    }

    FogManager::~FogManager() {
        if (this->receivedGrid)
        {
            delete[] fogArrayA;
            delete[] fogArrayB;
        }
    }

    void FogManager::receiveGrid(int w, int h)
    {
        this->receivedGrid = true;
        this->gridWidth = w;
        this->gridHeight = h;

        Oryol::String textureString = Oryol::String("images/fogTile.png");
        this->texture = InternalApp::getInternalApp()->resourceManager.textures[textureString];

        this->fogArrayA = initializeFogArray(this->gridWidth, this->gridHeight);
        this->fogArrayB = initializeFogArray(this->gridWidth, this->gridHeight);
    }

    int* FogManager::initializeFogArray(int width, int height) {
        int* output = new int[width * height];
        for (int j = 0; j < height; j += 1)
        {
            for (int i = 0;i< width; i+=1)
            {
                output[(j*width)+i] = fogState;
            }
        }
        return output;
    }

    int FogManager::getGridIndex(int x, int y) {
        if (!this->receivedGrid) return -1;
        if (x < 0 || y < 0 || x >= this->gridWidth*2 || y >= this->gridHeight*2) return -1;

        if (x % 2 == 0 && y % 2 == 0) return 0;
        if ((x+1) % 2 == 0 && (y+1) % 2 == 0) return 1;
        return -1;
    }

    int FogManager::getTileIndex(int gridIndex, int x, int y) {
        if (gridIndex == 0)
        {
            int xsmall = x / 2;
            int ysmall = y / 2;
            return (ysmall * this->gridWidth) + xsmall;
        }
        else
        {
            int xsmall = (x-1) / 2;
            int ysmall = (y-1) / 2;
            return (ysmall * this->gridWidth) + xsmall;
        }
    }

    void FogManager::revealFog(int x, int y, int radius)
    {
        this->updateFog(x,y,radius, true);
    }

    void FogManager::conceilFog(int x, int y, int radius) {
        this->updateFog(x,y,radius, false);
    }

    void FogManager::updateFog(int centerX, int centerY, int radius, bool reveal)
    {
        if (!this->receivedGrid) return;

        auto g = (Global*) InternalApp::getGlobal();

        auto sightCircle = g->circleCache->getCircle(radius);

        int inc = reveal ? 1 : -1;

        for (auto iterator = sightCircle->coordList->GetIterator(); iterator->Valid(); iterator->Next())
        {
            auto coord = iterator->Get();
            int x = centerX + coord->x;
            int y = centerY + coord->y;

            int gridIndex = getGridIndex(x, y);
            if (gridIndex == -1) continue;

            int tileIndex = getTileIndex(gridIndex, x, y);
            if (gridIndex == 0)
            {
                if (reveal && this->fogArrayA[tileIndex] == 0)
                {
                    this->fogArrayA[tileIndex] += 2;
                }
                else
                {
                    this->fogArrayA[tileIndex] += inc;
                }

            }
            else
            {
                if (reveal && this->fogArrayB[tileIndex] == 0)
                {
                    this->fogArrayB[tileIndex] += 2;
                }
                else
                {
                    this->fogArrayB[tileIndex] += inc;
                }
            }
        }
    }

    bool FogManager::tileIsInShroud(int x, int y)
    {
        return this->getFogAmount(x, y) == 0;
    }

    bool FogManager::tileIsInFog(int x, int y)
    {
        return this->getFogAmount(x, y) == 1;
    }

    bool FogManager::tileIsClear(int x, int y)
    {
        return this->getFogAmount(x, y) > 1;
    }

    int FogManager::getFogAmount(int x, int y)
    {
        if (!this->receivedGrid) return 0;
        int gridIndex = getGridIndex(x, y);
        if (gridIndex == -1) return 0;
        int tileIndex = getTileIndex(gridIndex, x, y);
        if (gridIndex == 0)
        {
            return this->fogArrayA[tileIndex];
        }
        else
        {
            return this->fogArrayB[tileIndex];
        }
    }

    ///Drawing

    void FogManager::Draw(Ref<Camera> camera, int xoffset, int yoffset)
    {
        if (this->destroyed) return;
        if (!this->receivedGrid) return;

        yoffset-=1;

        const auto resState = Oryol::Gfx::QueryResourceInfo(this->texture->textureId).State;
        if (resState == Oryol::ResourceState::Valid)
        {
            int a = xoffset / -tileWidth;
            int b = yoffset / -tileHeight;

            for (int j = -1; j < 13; j++) // j should take into account grid height... right?
            {
                for (int i = -1; i < 14; i += 1) // j should take into account grid height... right?
                {
                    int x = a + i;
                    int y = b + j;

                    if (getGridIndex(x * 2, y * 2) == -1)continue; // checking if out of bounds

                    if (tileIsInFog(x * 2, y * 2))
                    {
                        drawFogTile(camera, ((x) * tileWidth) + xoffset, ((y) * tileHeight) + yoffset);
                    }

                    if (tileIsInFog((x * 2)+1, (y * 2)+1)) {
                        drawFogTile(camera, ((x + 0.5f) * tileWidth) + xoffset + 1, ((y + 0.5f) * tileHeight) + yoffset);
                    }
                }
            }
        }
    }

    void FogManager::drawFogTile(Ref<Camera> camera, float xoffset, float yoffset)
    {
        Oryol::DrawState drawState = camera->getInternalCamera()->getDrawState();
        drawState.FSTexture[0] = this->texture->textureId;
        const void *data = this->updateVertices(xoffset, yoffset, this->texture->width, this->texture->height,
                                                camera->getInternalCamera()->getWidth(),
                                                camera->getInternalCamera()->getHeight(), 1);
        Oryol::Gfx::UpdateVertices(drawState.Mesh[0], data, InternalApp::numVertexesInQuad);
        Oryol::Gfx::ApplyDrawState(drawState);

        Oryol::Gfx::Draw();
    }

    const void * FogManager::updateVertices(float x, float y, int texWidth, int texHeight, int canWidth, int canHeight, int padding) {

        int vIndex = 0;

        //0 is 0, 1 is canvasWidth, canvasHeight
        float x0 = x / (float) canWidth;
        float y0 = y / (float) canHeight;
        float x1 = (20 - (padding*2) + x) / (float) canWidth;
        float y1 = (12 - (padding*2) + y) / (float) canHeight;

        //0 is 0, 1 is texWidth/texHeight
        //This is the texture
        float u0 = (float) (padding) / (float) texWidth;
        float v0 = (float) padding / (float) texHeight;
        float u1 = (float) (20 - padding) / (float) texWidth;
        float v1 = (float) (12 - padding) / (float) texHeight;

        vIndex = this->writeVertex(vIndex, x0, y0, u0, v0);
        vIndex = this->writeVertex(vIndex, x1, y0, u1, v0);
        vIndex = this->writeVertex(vIndex, x1, y1, u1, v1);
        vIndex = this->writeVertex(vIndex, x0, y0, u0, v0);
        vIndex = this->writeVertex(vIndex, x1, y1, u1, v1);
        vIndex = this->writeVertex(vIndex, x0, y1, u0, v1);

        return InternalApp::getInternalApp()->vertexBuffer;
    }

    int FogManager::writeVertex(int index, float x, float y, float u, float v) {
        InternalApp::getInternalApp()->vertexBuffer[index].x = x;
        InternalApp::getInternalApp()->vertexBuffer[index].y = y;
        InternalApp::getInternalApp()->vertexBuffer[index].u = u;
        InternalApp::getInternalApp()->vertexBuffer[index].v = v;
        return index + 1;
    }

    void FogManager::minimapDrawFog(Sova::Ref<Camera> camera, int xs, int ys, int w, int h)
    {
        if (this->destroyed) return;
        if (!this->receivedGrid) return;

        //clear section
        if (this->minimapRect->position->x == -1){
            this->minimapRect->size->set((w*2)+2,(h*2)+2);
        }
        this->minimapRect->position->set((xs*2),(ys*2));
        this->minimapRect->drawSelf(camera, 0, 0);

        for (int i = 0; i <= w; i++)
        {
            for (int j = 0; j <= h; j += 1)
            {
                int x = xs + i;
                int y = ys + j;

                if (getGridIndex(x * 2, y * 2) == -1) continue; // checking if out of bounds

                if (tileIsInFog(x * 2, y * 2))
                {
                    this->minimapPixel->position->set(x*2, y*2);
                    this->minimapPixel->drawSelf(camera, 0, 0);

                    this->minimapPixel->position->set((x*2)+1, y*2);
                    this->minimapPixel->drawSelf(camera, 0, 0);
                }

                if (tileIsInFog((x * 2)+1, (y * 2)+1))
                {
                    this->minimapPixel->position->set((x * 2)+1, (y * 2)+1);
                    this->minimapPixel->drawSelf(camera, 0, 0);

                    this->minimapPixel->position->set((x * 2)+2, (y * 2)+1);
                    this->minimapPixel->drawSelf(camera, 0, 0);
                }
            }
        }
    }

    void FogManager::shroudToFog(int x, int y) {
        if (this->getFogAmount(x, y) == 0)
        {
            int gridIndex = getGridIndex(x, y);
            if (gridIndex == -1) return;
            int tileIndex = getTileIndex(gridIndex, x, y);
            if (gridIndex == 0)
            {
                if (this->fogArrayA[tileIndex] == 0)
                {
                    this->fogArrayA[tileIndex] = 1;
                }
            }
            else
            {
                if (this->fogArrayB[tileIndex] == 0)
                {
                    this->fogArrayB[tileIndex] = 1;
                }
            }
        }
    }
}