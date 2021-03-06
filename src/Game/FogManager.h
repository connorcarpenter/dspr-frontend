#pragma once

//
// Created by connor on 8/16/18.
//

#include <Sova/Graphics/Container.h>
#include <Sova/Graphics/Internal/InternalTexture.h>
#include <Sova/Graphics/Pixel.h>
#include <Sova/Graphics/Rectangle.h>

namespace DsprFrontend
{
    class FogManager : public Container
    {

    public:

        FogManager();
        ~FogManager();

        void receiveGrid(int w, int h);
        void revealFog(int x, int y, int radius);
        void conceilFog(int x, int y, int radius);
        void shroudToFog(int x, int y);
        bool tileIsInShroud(int x, int y);

        void Draw(Ref<Camera> camera, int xoffset, int yoffset);
        void minimapDrawFog(Sova::Ref<Camera> camera, int x, int y, int w, int h);

    private:

        int gridWidth = 0;
        int gridHeight = 0;
        bool receivedGrid = false;
        int* fogArrayA = nullptr;
        int* fogArrayB = nullptr;
        const int tileWidth = 20;
        const int tileHeight = 10;
        const int fogState = 0;

        int * initializeFogArray(int width, int height);
        void updateFog(int x, int y, int radius, bool reveal);
        int getGridIndex(int x, int y);
        int getTileIndex(int gridIndex, int x, int y);

        int getFogAmount(int x, int y);
        bool tileIsClear(int x, int y);
        bool tileIsInFog(int x, int y);

        Sova::InternalTexture* texture = nullptr;
        Ref<Sova::Pixel> minimapPixel = Null<Pixel>();
        Ref<Sova::Rectangle> minimapRect = Null<Rectangle>();

        const void *updateVertices(float x, float y, int texWidth, int texHeight, int canWidth, int canHeight, int padding);
        int writeVertex(int index, float x, float y, float u, float v);
        void drawFogTile(Ref<Camera> camera, float xoffset, float yoffset);
    };
}