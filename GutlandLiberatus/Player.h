#pragma once

#include "Character.h"
#include <Kengine/EventManager.h>

class Player: public Character
{
public:
            Player(EntityManager * entityManager);
            ~Player();

    void    OnEntityCollision(EntityBase* collider, bool attack);
    void    React(Kengine::EventDetails* details);
};
