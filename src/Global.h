#pragma once

//
// Created by connor on 7/26/18.
//

#include <Sova/References/Ref.h>
#include <Sova/References/Refable.h>
#include <Sova/App.h>
#include "TileManager.h"
#include "Cursor.h"



namespace DsprFrontend
{
    class Unit;

    class Global : public Sova::Refable {
    public:
        Ref<App> app = Null<App>();
        Ref<Viewport> viewport = Null<Viewport>();
        Ref<Camera> camera = Null<Camera>();
        Ref<Container> world = Null<Container>();
        Ref<Websocket> bffServer = Null<Websocket>();
        Ref<Websocket> gameServer = Null<Websocket>();
        Ref<TileManager> tileManager = Null<TileManager>();
        Ref<Unit> worker = Null<Unit>();
        Ref<Cursor> cursor = Null<Cursor>();
        Ref<AnimatedSprite> unitHoverCircle = Null<AnimatedSprite>();
        Ref<Sprite> unitSelectCircle = Null<Sprite>();
        Ref<AnimatedSprite> moveMarker = Null<AnimatedSprite>();
    };
}