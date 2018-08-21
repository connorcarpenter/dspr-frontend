//
// Created by connor on 8/16/18.
//

#include <Sova/Graphics/Container.h>
#include <Sova/Graphics/Sprite.h>
#include <Sova/Graphics/AnimatedSprite.h>
#include <Sova/Graphics/Line.h>

namespace DsprFrontend
{
    class UiManager : public Container
    {
    public:
        UiManager();
        void Draw(Ref<Camera> camera, int xoffset, int yoffset);
        bool captureLeftClickEvent(Ref<Point> clickPoint);

        Ref<Point> getMinimapPosition(Ref<Point> clickPoint);

    private:
        Ref<Sprite> minimap = Null<Sprite>();
        Ref<Sprite> armybar = Null<Sprite>();
        Ref<Sprite> command = Null<Sprite>();
        Ref<Sprite> unitPortrait = Null<Sprite>();
        Ref<Sova::Line> healthBarLine = Null<Sova::Line>();
        Ref<AnimatedSprite> commandActionsUp = Null<AnimatedSprite>();
        Ref<AnimatedSprite> commandActionsHover = Null<AnimatedSprite>();

    };
}