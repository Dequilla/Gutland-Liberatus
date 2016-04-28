#pragma once

#include "Character.h"
#include <Kengine/EventManager.h>
#include <SFML/Audio.hpp>

class Player: public Character
{
public:
					Player(EntityManager * entityManager);
					~Player();

    void			OnEntityCollision(EntityBase* collider, bool attack);
    void			React(Kengine::EventDetails* details);

private:
	sf::Sound		m_stepSound;
	sf::SoundBuffer	m_stepSoundBuffer;
};
