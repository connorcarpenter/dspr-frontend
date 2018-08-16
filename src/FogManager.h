//
// Created by connor on 8/16/18.
//

#include <Sova/Graphics/Container.h>
#include <Sova/Graphics/Internal/InternalTexture.h>

namespace DsprFrontend
{
    class FogManager : public Container
    {
    public:
        FogManager();
        ~FogManager();
        void receiveGrid(int w, int h);
        void revealFog(int x, int y, int radius, bool reveal);

        bool tileIsInShroud(int x, int y);

        void Draw(Ref<Camera> camera, int xoffset, int yoffset);

    private:
        int gridWidth = 0;
        int gridHeight = 0;
        bool receivedGrid = false;
        int* fogArrayA = nullptr;
        int* fogArrayB = nullptr;
        const int tileWidth = 20;
        const int tileHeight = 10;

        int * initializeFogArray(int width, int height);

        int getGridIndex(int x, int y);

        int getTileIndex(int gridIndex, int x, int y);

        bool tileIsInFog(int x, int y);

        int getFogAmount(int x, int y);

        bool tileIsClear(int x, int y);

        Sova::InternalTexture* texture = nullptr;

        const void *updateVertices(float x, float y, int texWidth, int texHeight, int canWidth, int canHeight, int padding);

        int writeVertex(int index, float x, float y, float u, float v);

        void drawFogTile(Ref<Camera> camera, float xoffset, float yoffset);
    };
}