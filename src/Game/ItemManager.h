#pragma once

//
// Created by connor on 11/8/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>
#include <RefIsoGrid.h>

namespace DsprFrontend {
    class Item;
    class ItemManager : public Sova::Refable {
    public:
        ItemManager();
        void receiveItem(Ref<Sova::String> id, Ref<Sova::String> x, Ref<Sova::String> y,
                         Ref<Sova::String> itemIndexStr);
        void receiveItemDelete(Ref<Sova::String> idStr, Ref<Sova::String> propsStr);
        void receiveGrid(int w, int h);
    private:
        Ref<List<Item>> itemList = Null<List<Item>>();
        Ref<RefIsoGrid<Item>> itemGrid = Null<RefIsoGrid<Item>>();
        bool receivedGrid = false;
        int gridWidth;
        int gridHeight;

        void updateItemPosition(Ref<Item> item, Ref<Point> oldPosition, Ref<Point> newPosition);
    };
}