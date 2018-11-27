//
// Created by connor on 8/20/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/Pixel.h>

namespace DsprFrontend
{
    class Global;

    class Minimap : public Sova::Container
    {
    public:
        Minimap();
        void DrawTile(int x, int y, Color color);

        void DrawTile(int x, int y);

        void ReceiveGrid(int width, int height);

    private:
        Global* g = nullptr;
        Ref<Camera> terrainCamera = Null<Camera>();
        Ref<Sova::Pixel> pixel = Null<Sova::Pixel>();

        void Step(float deltaFrameMs);

        void initMinimap(int width, int height);

        bool receivedGrid = false;
    };
}