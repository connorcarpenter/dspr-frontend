//
// Created by connor on 11/8/18.
//

#include <Sova/Internal/InternalApp.h>
#include "ItemManager.h"
#include "Game/Global.h"
#include "Item.h"
#include "Game/TileManager.h"

namespace DsprFrontend {

    ItemManager::ItemManager() {
        this->itemList = New<List<Item>>();
    }

    void ItemManager::receiveItem(Ref<Sova::String> idStr, Ref<Sova::String> xStr, Ref<Sova::String> yStr,
                                                Ref<Sova::String> itemIndexStr) {
        int id = atoi(idStr->AsCStr());
        int x = atoi(xStr->AsCStr());
        int y = atoi(yStr->AsCStr());
        int index = atoi(itemIndexStr->AsCStr());

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();
        
        auto newItem = New<Item>(id, x, y, index);

        g->world->AddChild(newItem);
        this->itemList->Add(newItem);

        this->updateItemPosition(newItem, Null<Point>(), newItem->tilePosition);
    }

    void ItemManager::receiveItemDelete(Ref<Sova::String> idStr, Ref<Sova::String> propsStr) {
        int id = atoi(idStr->AsCStr());

        Ref<Item> item = this->itemList->Find([&](Ref<Item> inspectItem) { //replace this with a Hash lookup someday!
            return (inspectItem->id == id);
        });

        if (item == nullptr) return;

        this->itemList->Remove(item);
        this->updateItemPosition(item, item->tilePosition, Null<Point>());
        item->Destroy();
    }

    void ItemManager::updateItemPosition(Ref<Item> item, Ref<Point> oldPosition, Ref<Point> newPosition)
    {
        if (!this->receivedGrid) return;

        if (oldPosition != nullptr)
            this->itemGrid->set(oldPosition->x, oldPosition->y, Null<Item>());

        if (newPosition != nullptr)
            this->itemGrid->set(newPosition->x, newPosition->y, item);
    }

    void ItemManager::receiveGrid(int w, int h) {
        this->receivedGrid = true;
        this->gridWidth = w;
        this->gridHeight = h;

        this->itemGrid = New<RefIsoGrid<Item>>();
        this->itemGrid->initialize(this->gridWidth * 2, this->gridHeight * 2);
    }

    Ref<Item> ItemManager::getItemOverlappingWithPoint(int x, int y)
    {
        if (!this->receivedGrid) return Null<Item>();

        auto g = (Global*) InternalApp::getSovaApp()->getGlobal();

        auto tilePos = g->tileManager->getTilePosition(x, y);

        return this->itemGrid->get(tilePos->x, tilePos->y);
    }
}
