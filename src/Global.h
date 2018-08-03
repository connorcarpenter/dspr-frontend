#pragma once

//
// Created by connor on 7/26/18.
//

#include <Sova/References/Ref.h>
#include <Sova/References/Refable.h>
#include <Sova/App.h>
#include "TileManager.h"
#include "Cursor.h"
#include "UnitManager.h"
#include "NetworkManager.h"

namespace DsprFrontend
{
    class Unit;

    class Global : public Sova::Refable {
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
    };
}