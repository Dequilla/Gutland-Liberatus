#include "EntityManager.h"
#include "SharedContext.h"

EntityManager::EntityManager(SharedContext* context, unsigned int maxEntities) :
	m_context(context), m_maxEntities(maxEntities), m_idCounter(0)
{
	LoadEnemyTypes("EnemyList.list");
	RegisterEntity<Player>(EntityType::Player);
	RegisterEntity<Enemy>(EntityType::Enemy);
}

EntityManager::~EntityManager()
{
	Purge();
	m_entityFactory.clear();
	m_context = nullptr;
}

int EntityManager::Add(const EntityType& type, const std::string& name)
{
	auto itr = m_entityFactory.find(type);

	if (itr == m_entityFactory.end()) { return -1; }
	EntityBase* entity = itr->second();
	entity->m_id = m_idCounter;

	if (name != "") { entity->m_name = name; }
	m_entities.emplace(m_idCounter, entity);

	if (type == EntityType::Enemy)
	{
		auto itr = m_enemyTypes.find(name);

		if (itr != m_enemyTypes.end())
		{
			Enemy* enemy = (Enemy*)entity;
			enemy->Load(itr->second);
		}
	}

	++m_idCounter;
	return m_idCounter - 1;
}

EntityBase* EntityManager::Find(unsigned int id)
{
	auto itr = m_entities.find(id);
	if (itr == m_entities.end()) { return nullptr; }
	return itr->second;
}

EntityBase* EntityManager::Find(const std::string & name)
{
	for (auto &itr : m_entities)
	{
		if (itr.second->GetName() == name)
		{
			return itr.second;
		}
	}
	return nullptr;
}

void EntityManager::Remove(unsigned int id)
{
	m_entitiesToRemove.emplace_back(id);
}

void EntityManager::Update(float dt)
{
	for (auto &itr : m_entities)
	{
		itr.second->Update(dt);
	}
	EntityCollisionCheck();
	ProcessRemovals();
}

void EntityManager::Draw()
{
	sf::RenderWindow* window = m_context->window->GetRenderWindow();
	sf::FloatRect viewSpace = m_context->window->GetViewSpace();

	for (auto &itr : m_entities)
	{
		if (!viewSpace.intersects(itr.second->m_AABB)) { continue; }
		itr.second->Draw(window);
	}
}

void EntityManager::Purge()
{
	for (auto &itr : m_entities)
	{
		delete itr.second;
	}
	m_entities.clear();

	m_idCounter = 0;
}

SharedContext* EntityManager::GetContext()
{
	return m_context;
}

void EntityManager::ProcessRemovals()
{
	while (m_entitiesToRemove.begin() != m_entitiesToRemove.end())
	{
		unsigned int id = m_entitiesToRemove.back();
		auto itr = m_entities.find(id);
		if (itr != m_entities.end())
		{
			std::cout << "Discarding entity: " << itr->second->GetId() << std::endl;
			delete itr->second;
			m_entities.erase(itr);
		}
		m_entitiesToRemove.pop_back();
	}
}

void EntityManager::LoadEnemyTypes(const std::string& name)
{
	std::ifstream file;
	file.open(std::string("media/Characters/") + name);

	if (!file.is_open())
	{
		std::cout << "Failed loading file: " << name << std::endl;
		return;
	}

	std::string line;

	while (std::getline(file, line))
	{
		if (line[0] == '|') { continue; }
		std::stringstream keystream(line);
		std::string charName;
		std::string charFile;
		keystream >> charName >> charFile;
		m_enemyTypes.emplace(charName, charFile);
	}
	file.close();
}

void EntityManager::EntityCollisionCheck()
{
	if (m_entities.empty()) { return; }

	for (auto itr = m_entities.begin();
	std::next(itr) != m_entities.end(); ++itr)
	{
		for (auto itr2 = std::next(itr);
		itr2 != m_entities.end(); ++itr2)
		{
			if (itr->first == itr2->first) { continue; }

			// Regular AABB bounding box collision.
			if (itr->second->m_AABB.intersects(itr2->second->m_AABB))
			{
				itr->second->OnEntityCollision(itr2->second, false);
				itr2->second->OnEntityCollision(itr->second, false);
			}

			EntityType t1 = itr->second->GetType();
			EntityType t2 = itr2->second->GetType();

			if (t1 == EntityType::Player || t1 == EntityType::Enemy)
			{
				Character* c1 = (Character*)itr->second;

				if (c1->m_attackAABB.intersects(itr2->second->m_AABB))
				{
					c1->OnEntityCollision(itr2->second, true);
				}
			}

			if (t2 == EntityType::Player || t2 == EntityType::Enemy)
			{
				Character* c2 = (Character*)itr2->second;

				if (c2->m_attackAABB.intersects(itr->second->m_AABB))
				{
					c2->OnEntityCollision(itr->second, true);
				}
			}
		}
	}
}
