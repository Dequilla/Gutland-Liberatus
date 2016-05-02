#include "Enemy.h"

Enemy::Enemy(EntityManager* entityManager) :
    Character(entityManager), m_hasDestination(false)
{
    m_type = EntityType::Enemy;
}

Enemy::~Enemy()
{
}

void Enemy::OnEntityCollision(EntityBase* collider, bool attack)
{
    if (m_state == EntityState::Dying)
    {
        return;
    }
    if (attack)
    {
        return;
    }
    if (collider->GetType() != EntityType::Player)
    {
        return;
    }
    Character* player = (Character *) collider;
    if (m_position.x > player->GetPosition().x)
    {
        player->AddVelocity(-m_speed.x, 0);
        m_spriteSheet.SetDirection(Kengine::Direction::Left);
    }
    else if (m_position.x < player->GetPosition().x)
    {
        player->AddVelocity(m_speed.x, 0);
        m_spriteSheet.SetDirection(Kengine::Direction::Right);
    }
    else if (m_position.y > player->GetPosition().y)
    {
        player->AddVelocity(0, -m_speed.y);
        m_spriteSheet.SetDirection(Kengine::Direction::Up);
    }
    else
    {
        player->AddVelocity(0, m_speed.y);
        m_spriteSheet.SetDirection(Kengine::Direction::Down);
    }
}

void Enemy::Update(float dt)
{
    Character::Update(dt);

    if (m_hasDestination)
    {
        if ((abs(m_destination.x - m_position.x) < 16) ||
            (abs(m_destination.y - m_position.y) < 16))
        {
            m_hasDestination = false;
            return;
        }

        if (m_destination.x - m_position.x > 0)
        {
            Move(Kengine::Direction::Right);
        }
        if (m_destination.x - m_position.x < 0)
        {
            Move(Kengine::Direction::Left);
        }
        if (m_destination.y - m_position.y < 0)
        {
            Move(Kengine::Direction::Up);
        }
        if (m_destination.y - m_position.y > 0)
        {
            Move(Kengine::Direction::Down);
        }

        if (m_collidingOnX || m_collidingOnY)
        {
            m_hasDestination = false;
        }
        return;
    }
    int random = rand() % 100 + 1;
    if (random != 100)
    {
        return;
    }
    int newPoint = rand() % 65 + 0;

    if (rand() % 2)
    {
        newPoint = -newPoint;
    }

    int dir = rand() % 4 + 1;
    if (dir == 1)
    {
        m_destination.x = m_position.x + newPoint;
    }
    else if(dir == 2)
    {
        m_destination.x = m_position.x - newPoint;
    }
    else if (dir == 3)
    {
        m_destination.y = m_position.y + newPoint;
    }
    else if (dir == 4)
    {
        m_destination.y = m_position.y - newPoint;
    }

//    m_destination.x = m_position.x + newX;
    if (m_destination.x < 0)
    {
        m_destination.x = 0;
    }
    if (m_destination.y < 0)
    {
        m_destination.y = 0;
    }
    m_hasDestination = true;
}
