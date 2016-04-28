#include "Player.h"
#include "EntityManager.h"
#include "StateManager.h"

Player::Player(EntityManager* entityManager) :
    Character(entityManager)
{
    Load("Player.char");
    m_type = EntityType::Player;

    Kengine::EventManager* events = m_entityManager->GetContext()->eventManager;

    events->AddCallback<Player>(StateType::Game, "Player_MoveLeft",
                                &Player::React, this);
    events->AddCallback<Player>(StateType::Game, "Player_MoveRight",
                                &Player::React, this);
    events->AddCallback<Player>(StateType::Game, "Player_MoveUp",
                                &Player::React, this);
    events->AddCallback<Player>(StateType::Game, "Player_MoveDown",
                                &Player::React, this);
    this->Move(Kengine::Direction::Down);

	if (!m_stepSoundBuffer.loadFromFile("media/Sound/Effects/Steps/leaves01.ogg"))
	{
		std::cout << "Could not load gravel.ogg" << std::endl;
	}

	m_stepSound.setBuffer(m_stepSoundBuffer);
	m_stepSound.setLoop(false);
}

Player::~Player()
{
    Kengine::EventManager* events = m_entityManager->GetContext()->eventManager;

    events->RemoveCallback(StateType::Game, "Player_MoveLeft");
    events->RemoveCallback(StateType::Game, "Player_MoveRight");
    events->RemoveCallback(StateType::Game, "Player_MoveUp");
    events->RemoveCallback(StateType::Game, "Player_MoveDown");
}

void Player::OnEntityCollision(EntityBase* collider, bool attack)
{
    if (m_state == EntityState::Dying)
    {
        return;
    }
    if (attack)
    {
        if (m_state != EntityState::Attacking)
        {
            return;
        }
        if (!m_spriteSheet.GetCurrentAnimation()->IsInAction())
        {
            return;
        }
        if (collider->GetType() != EntityType::Enemy &&
            collider->GetType() != EntityType::Player)
        {
            return;
        }
        Character* opponent = (Character *) collider;
        opponent->GetHurt(1);
        if (m_position.x > opponent->GetPosition().x)
        {
            opponent->AddVelocity(-32, 0);
        }
        else
        {
            opponent->AddVelocity(32, 0);
        }
    }
    else
    {
        // Other behavior.
    }
}

void Player::React(Kengine::EventDetails* details)
{
    if (details->m_name == "Player_MoveLeft")
    {
        Character::Move(Kengine::Direction::Left);
    }
    else if (details->m_name == "Player_MoveRight")
    {
        Character::Move(Kengine::Direction::Right);
    }
    else if (details->m_name == "Player_MoveUp")
    {
        Character::Move(Kengine::Direction::Up);
    }
    else if (details->m_name == "Player_MoveDown")
    {
        Character::Move(Kengine::Direction::Down);
    }
    else if (details->m_name == "Player_Jump")
    {
        Character::Jump();
    }
    else if (details->m_name == "Player_Attack")
    {
        Character::Attack();
    }

	if (m_stepSound.getStatus() == m_stepSound.Stopped && 
		m_spriteSheet.GetCurrentAnimation()->GetName() == "Walk")
	{
		m_stepSound.play();
	}
}
