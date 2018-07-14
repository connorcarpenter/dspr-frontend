//
// Created by connor on 7/13/18.
//

#include "Sova/App.h"
#include "Sova/Math.h"

#include "MoveSprite.h"

using namespace DsprFrontend;

namespace DsprFrontend
{
    MoveSprite::MoveSprite(Ref<App> app)
    {
        float random = Math::Random();
        if (random < 0.3f)
        {
            this->setTexture(NewRef<String>("images/cat.png"));
        }
        else
        {
            this->setTexture(NewRef<String>("images/mouse.png"));
        }


        this->velocity = NewRef<Point>();
        float direction = Math::Random() * 359;
        float speed = (4 + 2 * Math::Random());
        this->velocity->x = (int) (Math::Cos(direction) * speed);
        this->velocity->y = (int) (Math::Sin(direction) * speed);

        this->app = app;
        this->position = NewRef<Point>(Math::Random() * app->width/4, Math::Random() * app->height/4);

        this->OnUpdate([&]() {
            this->Update();
        });
    }

    void MoveSprite::Update()
    {
        this->position->x += this->velocity->x;
        this->position->y += this->velocity->y;

        if (this->position->x + this->getWidth() < 0 && this->velocity->x < 0) this->velocity->x *= -1;//this->Destroy();
        if (this->position->y + this->getHeight() < 0 && this->velocity->y < 0) this->velocity->y *= -1;//this->Destroy();
        if (this->position->x > this->app->width / 2 && this->velocity->x > 0) this->velocity->x *= -1;//this->Destroy();
        if (this->position->y > this->app->height / 2 && this->velocity->y > 0) this->velocity->y *= -1;//this->Destroy();
    }
}