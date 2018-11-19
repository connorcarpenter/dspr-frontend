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
        void receiveItem(int id, int x, int y,
                         int index);
        void receiveItemDelete(Ref<Sova::String> idStr, Ref<Sova::String> propsStr);
        void receiveGrid(int w, int h);

        Ref<Item> getItemOverlappingWithPoint(int x, int y);

    private:
        Ref<List<Item>> itemList = Null<List<Item>>();
        Ref<RefIsoGrid<Item>> itemGrid = Null<RefIsoGrid<Item>>();
        bool receivedGrid = false;
        int gridWidth;
        int gridHeight;

        void updateItemPosition(Ref<Item> item, Ref<Point> oldPosition, Ref<Point> newPosition);

    };
}