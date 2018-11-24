//
// Created by connor on 8/20/18.
//

#include <Sova/Graphics/Container.h>
#include <Sova/Graphics/Pixel.h>
#include <Sova/Graphics/Rectangle.h>

namespace DsprFrontend
{
    class Global;

    class MinimapUnits : public Sova::Container
    {
    public:
        MinimapUnits();
        void drawSelf(Sova::Ref<Camera> camera, int xoffset, int yoffset) override;
    private:
        Global* g = nullptr;
        Ref<Sova::Rectangle> rectangle = Null<Sova::Rectangle>();

        void drawTile(Ref<Camera> camera, int x, int y);

        void drawTile(Ref<Camera> camera, int x, int y, Color color);
    };
}