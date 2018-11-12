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
    class UnitTemplateCatalog;
    class IsoBoxCache;
    class EconomyManager;
    class GraphicsManager;
    class ItemManager;
    class ShadowManager;

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
        Ref<NetworkManager> networkManager = Null<NetworkManager>();
        Ref<FogManager> fogManager = Null<FogManager>();
        Ref<UiManager> uiManager = Null<UiManager>();
        Ref<Minimap> minimap = Null<Minimap>();
        Ref<SpriteCatalog> spriteCatalog = Null<SpriteCatalog>();
        Ref<CircleCache> circleCache = Null<CircleCache>();
        Ref<IsoBoxCache> isoBoxCache = Null<IsoBoxCache>();
        Ref<ButtonCardCatalog> buttonCardCatalog = Null<ButtonCardCatalog>();
        Ref<UnitTemplateCatalog> unitTemplateCatalog = Null<UnitTemplateCatalog>();
        Ref<EconomyManager> economyManager = Null<EconomyManager>();
        Ref<GraphicsManager> graphicsManager = Null<GraphicsManager>();
        Ref<ItemManager> itemManager = Null<ItemManager>();
        Ref<ShadowManager> shadowManager = Null<ShadowManager>();

        Ref<AnimatedSprite> moveMarker = Null<AnimatedSprite>();
        Ref<AnimatedSprite> unitHoverCircle_1x1 = Null<AnimatedSprite>();
        Ref<Sprite> unitSelectCircle_1x1 = Null<Sprite>();
        Ref<AnimatedSprite> unitHoverCircle_5x5 = Null<AnimatedSprite>();
        Ref<Sprite> unitSelectCircle_5x5 = Null<Sprite>();
        Ref<AnimatedSprite> unitHoverCircle_3x3 = Null<AnimatedSprite>();
        Ref<Sprite> unitSelectCircle_3x3 = Null<Sprite>();
        Ref<AnimatedSprite> unitHoverCircle_2x2 = Null<AnimatedSprite>();
        Ref<Sprite> unitSelectCircle_2x2 = Null<Sprite>();

        int playersTribeIndex = -1;
    };
}