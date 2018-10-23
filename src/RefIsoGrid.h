#pragma once

//
// Created by connor on 10/21/18.
//

#include <assert.h>
#include <functional>
#include <Sova/References/Ref.h>
#include <Sova/Common/Array.h>

namespace DsprFrontend
{
    template<class T>
    class RefIsoGrid : public Sova::Refable
    {
    public:

        RefIsoGrid() {};

        void initialize(int width, int height)
        {
            assert(!this->initialized);
            assert(width % 2 == 0);
            assert(height % 2 == 0);

            this->initialized = true;
            this->width = width;
            this->height = height;
            this->halfWidth = width/2;
            this->halfHeight = height/2;
            this->gridArrayA = New<Sova::Array<T>>(this->halfWidth*this->halfHeight);
            this->gridArrayB = New<Sova::Array<T>>(this->halfWidth*this->halfHeight);
        }

        void set(int x, int y, Sova::Ref<T> obj)
        {
            int gridIndex = getGridIndex(x, y);
            if (gridIndex == -1) return;
            int tileIndex = getTileIndex(gridIndex, x, y);
            if (gridIndex == 0)
            {
                this->gridArrayA->Set(tileIndex, obj);
            }
            else
            {
                this->gridArrayB->Set(tileIndex, obj);
            }
        }

        Sova::Ref<T> get(int x, int y)
        {
            int gridIndex = getGridIndex(x, y);
            if (gridIndex == -1) return Null<T>();
            int tileIndex = getTileIndex(gridIndex, x, y);
            if (gridIndex == 0)
            {
                return this->gridArrayA->Get(tileIndex);
            }
            else
            {
                return this->gridArrayB->Get(tileIndex);
            }
        }

        void forEachCoord(std::function<void(int x, int y)> coordFunc) {
            for (int j = 0; j < halfHeight; j += 1)
            {
                for (int i = 0;i< halfWidth; i+=1)
                {
                    coordFunc(i*2, j*2);
                }

                for (int i = 0;i< halfWidth; i+=1)
                {
                    coordFunc((i*2)+1, (j*2)+1);
                }
            }
        }

        void forEachElement(std::function<void(T obj, int x, int y)> elFunc) {
            for (int j = 0; j < halfHeight; j += 1)
            {
                for (int i = 0;i< halfWidth; i+=1)
                {
                    elFunc(this->get(i*2, j*2), i*2, j*2);
                }

                for (int i = 0;i< halfWidth; i+=1)
                {
                    elFunc(this->get((i*2)+1, (j*2)+1), (i*2)+1, (j*2)+1);
                }
            }
        }

        int width = -1;
        int height = -1;
        bool initialized = false;

    private:

        int getGridIndex(int x, int y)
        {
            if (x < 0 || y < 0 || x >= this->width || y >= this->width) return -1;
            if (x % 2 == 0 && y % 2 == 0) return 0;
            if ((x+1) % 2 == 0 && (y+1) % 2 == 0) return 1;
            return -1;
        }

        int getTileIndex(int gridIndex, int x, int y)
        {
            if (gridIndex == 0)
            {
                int xsmall = x / 2;
                int ysmall = y / 2;
                return (ysmall * (this->width/2)) + xsmall;
            }
            else
            {
                int xsmall = (x-1) / 2;
                int ysmall = (y-1) / 2;
                return (ysmall * (this->width/2)) + xsmall;
            }
        }

        Ref<Sova::Array<T>> gridArrayA = Null<Sova::Array<T>>();
        Ref<Sova::Array<T>> gridArrayB = Null<Sova::Array<T>>();
        int halfWidth;
        int halfHeight;
    };
}