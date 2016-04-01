#pragma once

#include <functional>
#include <unordered_map>
#include "Player.h"
#include "Enemy.h"

using EntityContainer = std::unordered_map<unsigned int, EntityBase*>;
using EntityFactory = std::unordered_map<EntityType, std::function<EntityBase*(void) >> ;
//using EntityFactory = std::unordered_map<EntityType, std::function<EntityBase*(void)>>;
using EnemyTypes = std::unordered_map<std::string, std::string>;

struct SharedContext;

class EntityManager
{
public:
	EntityManager(SharedContext* context, unsigned int maxEntities);
	EntityManager();

	int Add(const EntityType& type, const std::string& name = "");
	EntityBase* Find(unsigned int id);
	EntityBase* Find(const std::string& name);

	void Remove(unsigned int id);

	void Update(float dt);
	void Draw();

	void Purge();

	SharedContext* GetContext();

private:
	EntityContainer m_entities;
	EnemyTypes m_enemyTypes;
	EntityFactory m_entityFactory;
	SharedContext* m_context;
	unsigned int m_idCounter;
	unsigned int m_maxEntities;

	std::vector<unsigned int> m_entitiesToRemove;

	template<class T>
	void RegisterEntity(const EntityType& type)
	{
		m_entityFactory[type] = [this]() -> EntityBase*
		{
			return new T(this);
		};
	}

	void ProcessRemovals();
	void LoadEnemyTypes(const std::string& name);
	void EntityCollisionCheck();
};