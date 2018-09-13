#pragma once

//
// Created by connor on 7/26/18.
//

#include <Sova/References/Ref.h>
#include <Sova/References/Refable.h>
#include <Sova/App.h>

namespace Sova
{
    class Sprite;
    class AnimatedSprite;
}

namespace DsprFrontend
{
    class Unit;
    class FogManager;
    class UiManager;
    class SpriteCatalog;
    class Minimap;
    class NetworkManager;
    class UnitManager;
    class Cursor;
    class TileManager;
    class CircleCache;
    class ButtonCardCatalog;

    class Global : public Sova::Refable
    {
    public:
        Ref<Sova::App> app = Null<Sova::App>();
        Ref<Viewport> viewport = Null<Viewport>();
        Ref<Camera> camera = Null<Camera>();
        Ref<Container> world = Null<Container>();
        Ref<Websocket> bffServer = Null<Websocket>();
        Ref<Websocket> gameServer = Null<Websocket>();
        Ref<Sova::String> gameServerPlayerToken = Null<Sova::String>();
        Ref<TileManager> tileManager = Null<TileManager>();
        Ref<UnitManager> unitManager = Null<UnitManager>();
        Ref<Cursor> cursor = Null<Cursor>();
        Ref<AnimatedSprite> unitHoverCircle = Null<AnimatedSprite>();
        Ref<Sprite> unitSelectCircle = Null<Sprite>();
        Ref<AnimatedSprite> moveMarker = Null<AnimatedSprite>();
        Ref<NetworkManager> networkManager = Null<NetworkManager>();
        Ref<FogManager> fogManager = Null<FogManager>();
        Ref<UiManager> uiManager = Null<UiManager>();
        Ref<Minimap> minimap = Null<Minimap>();
        Ref<SpriteCatalog> spriteCatalog = Null<SpriteCatalog>();
        Ref<CircleCache> circleCache = Null<CircleCache>();
        Ref<ButtonCardCatalog> buttonCardCatalog = Null<ButtonCardCatalog>();
        Ref<UnitTemplateCatalog> unitTemplateCatalog = Null<UnitTemplateCatalog>();
        int playersTribeIndex = -1;
    };
}