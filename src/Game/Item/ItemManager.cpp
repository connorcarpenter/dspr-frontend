//
// Created by connor on 11/8/18.
//

#include <Sova/Internal/InternalApp.h>
#include "ItemManager.h"
#include "Game/Global.h"
#include "Item.h"
#include "Game/TileManager.h"
#include "ItemTemplateCatalog.h"

namespace DsprFrontend {

    ItemManager::ItemManager() {
        this->itemList = New<List<Item>>();
    }

    void ItemManager::receiveItem(int id, int x, int y,
                                  int index) {

        auto g = (Global*) InternalApp::getGlobal();
        
        auto newItem = New<Item>(id, x, y, g->itemTemplateCatalog->findTemplateByIndex(index));

        g->world->AddChild(newItem);
        this->itemList->Add(newItem);

        this->updateItemPosition(newItem, Null<Point>(), newItem->tilePosition);
    }

    void ItemManager::receiveItemDelete(int id) {

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

        auto g = (Global*) InternalApp::getGlobal();

        auto tilePos = g->tileManager->getTilePosition(x, y);

        return this->itemGrid->get(tilePos->x, tilePos->y);
    }

    Ref<Item> ItemManager::getItemAtCoord(int x, int y) {
        if (!this->receivedGrid) return Null<Item>();

        return this->itemGrid->get(x,y );
    }
}
