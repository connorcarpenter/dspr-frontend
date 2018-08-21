//
// Created by connor on 8/20/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Graphics/Pixel.h>

namespace DsprFrontend
{
    class Global;

    class Minimap : public Sova::Refable
    {
    public:
        Minimap();
        void DrawTile(int x, int y, Color color);

        void DrawTile(int x, int y);

    private:
        Global* g = nullptr;
        Ref<Camera> terrainCamera = Null<Camera>();
        Ref<Sova::Pixel> pixel = Null<Sova::Pixel>();
    };
}