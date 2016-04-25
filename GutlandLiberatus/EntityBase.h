#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


enum class EntityType { Base, Enemy, Player };

enum class EntityState
{
    Idle, Walking, Jumping, Attacking, Hurt, Dying
};

struct TileInfo;

struct CollisionElement
{
    CollisionElement(float area, TileInfo * info,
                     const sf::FloatRect & bounds) : m_area(area),
        m_tile(info), m_tileBounds(bounds)
    {
    }

    ~CollisionElement();

    float         m_area;
    TileInfo      *m_tile;
    sf::FloatRect m_tileBounds;
};

using Collisions = std::vector < CollisionElement >;

class EntityManager;

class EntityBase
{
    friend class EntityManager;
public:
    EntityBase(EntityManager * entityManager);
    virtual ~EntityBase();

    // ... Getters/setters
    void SetPosition(const float& x, const float& y);
    void SetPosition(const sf::Vector2f& pos);
    void SetSize(const float& x, const float& y);
    void SetState(const EntityState& state);

    std::string GetName()
    {
        return m_name;
    }
    unsigned int GetId()
    {
        return m_id;
    }
    EntityType GetType()
    {
        return m_type;
    }
    EntityState GetState()
    {
        return m_state;
    }
    sf::Vector2f GetPosition()
    {
        return m_position;
    }
    sf::FloatRect GetAABB()
    {
        return m_AABB;
    }

    void Move(float x, float y);
    void AddVelocity(float x, float y);
    void Accelerate(float x, float y);
    void SetAcceleration(float x, float y);
    void ApplyFriction(float x, float y);

    virtual void Update(float dt);
    virtual void Draw(sf::RenderWindow* window) = 0;

protected:
    // Data members.
    std::string   m_name;
    EntityType    m_type;
    unsigned int  m_id;              // Entity id in the entity manager.

    sf::Vector2f  m_position;        // Current position.
    sf::Vector2f  m_positionOld;     // Position before entity moved.
    sf::Vector2f  m_velocity;        // Current velocty
    sf::Vector2f  m_maxVelocity;     // Maximum velocity.
    sf::Vector2f  m_speed;           // Value of acceleration.
    sf::Vector2f  m_acceleration;    // Current acceleration.
    sf::Vector2f  m_friction;        // Default friction value.
    TileInfo      * m_referenceTile; // Tile underneath entity.
    sf::Vector2f  m_size;            // Size of the collision box.
    sf::FloatRect m_AABB;            // The bounding box for collisions.
    EntityState   m_state;           // Current entity state

    // Flags for remembering axis collisions.
    bool         m_collidingOnX;
    bool         m_collidingOnY;
    Collisions   m_collisions;
    EntityManager* m_entityManager;

    // Functions
    void UpdateAABB();
    void CheckCollisions();
    void ResolveCollisions();

    //bool SortCollisions(const CollisionElement& first, const CollisionElement& second);

    // This function checks what THIS entity does to the collider entity.
    virtual void OnEntityCollision(EntityBase* collider, bool attack) = 0;
};
