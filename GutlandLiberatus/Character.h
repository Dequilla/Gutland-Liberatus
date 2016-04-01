#pragma once

#include "EntityBase.h"
#include "SpriteSheet.h"

class Character : public EntityBase
{
	friend class EntityManager;
public:
	Character(EntityManager* entityManager);
	virtual ~Character();

	void Move(const Direction& dir);
	void Jump();
	void Attack();
	void GetHurt(const int& damage);
	void Load(const std::string& path);

	virtual void OnEntityCollision(EntityBase* collider, bool attack) = 0;
	virtual void Update(float dt);
	void Draw(sf::RenderWindow* window);

protected:
	SpriteSheet m_spriteSheet;
	float m_jumpVelocity;
	int m_hitpoints;
	sf::FloatRect m_attackAABB;
	sf::Vector2f m_attackAABBoffset;

	void UpdateAttackAABB();
	void Animate();
};