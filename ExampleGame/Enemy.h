#pragma once

#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(EntityManager* entityManager);
	~Enemy();

	void OnEntityCollision(EntityBase* collider, bool attack);
	void Update(float dt);

private:
	sf::Vector2f m_destination;
	bool m_hasDestination;
};