#pragma once

#include "EntityBase.h"
#include <Kengine/SpriteSheet.h>
/*
   ===========================================================
   class Character is the parent class for all types of
   characters, such as Player, NPC, Monster, etc.
   ===========================================================
 */
class Character: public EntityBase
{
    friend class EntityManager;
public:
                        Character(EntityManager * entityManager);
    virtual             ~Character();

    void                Move(const Kengine::Direction& dir);
    void                Jump();
    void                Attack();
    void                GetHurt(const int& damage);
    void                Load(const std::string& path);

    virtual void        OnEntityCollision(EntityBase* collider, bool attack) = 0;
    virtual void        Update(float dt);
    void                Draw(sf::RenderWindow* window);

protected:
    Kengine::SpriteSheet m_spriteSheet;
    float                m_jumpVelocity;
    int                  m_hitpoints;
    sf::FloatRect        m_attackAABB;
    sf::FloatRect        m_boundingBox;
    sf::Vector2f         m_attackAABBoffset;

    void UpdateAttackAABB();
    void Animate();
};
