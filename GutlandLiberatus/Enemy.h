#pragma once

#include "Character.h"

/*
   ===========================================================
   class Enemy is a type of character that is unfriendly with
   the player character. It holds all kind of enemy types and
   handles basic AI functionality.
   ===========================================================
 */
class Enemy: public Character
{
public:
            Enemy(EntityManager * entityManager);
            ~Enemy();

    void    OnEntityCollision(EntityBase* collider, bool attack);
    void    Update(float dt);

private:
    sf::Vector2f m_destination;
    bool         m_hasDestination;
};
