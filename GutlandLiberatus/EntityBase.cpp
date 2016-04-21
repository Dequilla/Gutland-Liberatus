#include "EntityBase.h"
#include "EntityManager.h"
#include "SharedContext.h"
#include "Layer.h"
#include "Map.h"

bool SortCollisions(const CollisionElement& first, const CollisionElement& second)
{
	return first.m_area > second.m_area;
}

EntityBase::EntityBase(EntityManager* entityManager) :
	m_entityManager(entityManager), m_name("BaseEntity"),
	m_type(EntityType::Base), m_referenceTile(nullptr),
	m_state(EntityState::Idle), m_id(0), 
	m_collidingOnX(false), m_collidingOnY(false)
{
}

EntityBase::~EntityBase()
{
	for (auto &itr : m_collisions)
	{
		delete &itr;
	}
	m_collisions.clear();

	m_entityManager = nullptr;
}

void EntityBase::SetPosition(const float& x, const float& y)
{
	m_position = sf::Vector2f(x, y);
	UpdateAABB();
}

void EntityBase::SetPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	UpdateAABB();
}

void EntityBase::SetSize(const float& x, const float& y)
{
	m_size = sf::Vector2f(x, y);
	UpdateAABB();
}

void EntityBase::SetState(const EntityState& state)
{
	if (m_state == EntityState::Dying) { return; }
	m_state = state;
}

void EntityBase::Move(float x, float y)
{
	m_positionOld = m_position;
	m_position += sf::Vector2f(x, y);
	sf::Vector2u mapSize = m_entityManager->GetContext()->gameMap->GetMapSize();
	if (m_position.x < 0)
	{
		m_position.x = 0;
	}
	else if (m_position.x >(mapSize.x) * Sheet::Tile_Size)
	{
		m_position.x = (mapSize.x) * Sheet::Tile_Size;
	}

	if (m_position.y < 26)
	{
		m_position.y = 26;
	}
	else if (m_position.y > (mapSize.y) * Sheet::Tile_Size)
	{
		m_position.y = (mapSize.y) * Sheet::Tile_Size;
	}

	UpdateAABB();
}

void EntityBase::AddVelocity(float x, float y)
{
	m_velocity += sf::Vector2f(x, y);

	if (abs(m_velocity.x) > m_maxVelocity.x)
	{
		if (m_velocity.x < 0) { m_velocity.x = -m_maxVelocity.x; }
		else { m_velocity.x = m_maxVelocity.x; }
	}

	if (abs(m_velocity.y) > m_maxVelocity.y)
	{
		if (m_velocity.y < 0) { m_velocity.y = -m_maxVelocity.y; }
		else { m_velocity.y = m_maxVelocity.y; }
	}
}

void EntityBase::Accelerate(float x, float y)
{
	m_acceleration += sf::Vector2f(x, y);
}

void EntityBase::SetAcceleration(float x, float y)
{
	m_acceleration = sf::Vector2f(x, y);
}

void EntityBase::ApplyFriction(float x, float y)
{
	if (m_velocity.x != 0)
	{
		if (abs(m_velocity.x) - abs(x) < 0) { m_velocity.x = 0; }
		else
		{
			if (m_velocity.x < 0) { m_velocity.x += x; }
			else { m_velocity.x -= x; }
		}
	}

	if (m_velocity.y != 0)
	{
		if (abs(m_velocity.y) - abs(y) < 0) { m_velocity.y = 0; }
		else
		{
			if (m_velocity.y < 0) { m_velocity.y += y; }
			else { m_velocity.y -= y; }
		}
	}
}

void EntityBase::Update(float dt)
{
	Map* map = m_entityManager->GetContext()->gameMap;
	float gravity = map->GetGravity();
	Accelerate(0, gravity);
	AddVelocity(m_acceleration.x * dt, m_acceleration.y * dt);
	SetAcceleration(0.0f, 0.0f);
	sf::Vector2f frictionValue;
	if (m_referenceTile)
	{
		frictionValue = m_referenceTile->m_friction;
		if (m_referenceTile->m_deadly) { SetState(EntityState::Dying); }
	}
	else if (map->GetDefaultTile())
	{
		frictionValue = map->GetDefaultTile()->m_friction;
	}
	else
	{
		frictionValue = m_friction;
	}

	float friction_x = (m_speed.x * frictionValue.x) * dt;
	float friction_y = (m_speed.y * frictionValue.y) * dt;
	ApplyFriction(friction_x, friction_y);
	sf::Vector2f deltaPos = m_velocity * dt;
	Move(deltaPos.x, deltaPos.y);
	m_collidingOnX = false;
	m_collidingOnY = false;
	CheckCollisions();
	ResolveCollisions();
}

void EntityBase::UpdateAABB()
{
	m_AABB = sf::FloatRect(m_position.x - (m_size.x / 2),
						   m_position.y - m_size.y, 
						   m_size.x, m_size.y);
}

void EntityBase::CheckCollisions()
{
	Map* gameMap = m_entityManager->GetContext()->gameMap;
	std::vector<Layer*>* mapLayer = m_entityManager->GetContext()->gameMap->GetLayers();
	unsigned int tileSize = gameMap->GetTileSize();
	int fromX = floor(m_AABB.left / tileSize);
	int toX = floor((m_AABB.left + m_AABB.width) / tileSize);
	int fromY = floor(m_AABB.top / tileSize);
	int toY = floor((m_AABB.top + m_AABB.height) / tileSize);

	for (auto itr = mapLayer->begin(); itr != mapLayer->end(); ++itr)
	{
		if ((*itr)->GetLayerName() == std::string("collision"))
		{
			for (int x = fromX; x <= toX; ++x)
			{
				for (int y = fromY; y <= toY; ++y)
				{
					Tile* tile = (*itr)->GetTile(x, y);
					if (!tile) { continue; }
					sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);
					sf::FloatRect intersection;
					m_AABB.intersects(tileBounds, intersection);
					float area = intersection.width * intersection.height;

					CollisionElement e(area, tile->properties, tileBounds);
					m_collisions.emplace_back(e);
					if (tile->warp && m_type == EntityType::Player)
					{
						gameMap->LoadNext(tile->properties->warpValue);
					}
				}
			}
		}
	}
}

void EntityBase::ResolveCollisions()
{
	if (!m_collisions.empty())
	{
		std::sort(m_collisions.begin(), m_collisions.end(), SortCollisions);
		Map* gameMap = m_entityManager->GetContext()->gameMap;
		unsigned int tileSize = gameMap->GetTileSize();
		for (auto &itr : m_collisions)
		{
			if (!m_AABB.intersects(itr.m_tileBounds)) { continue; }
			// Debug
			if (m_entityManager->GetContext()->debugOverlay.Debug())
			{
				sf::Vector2f tempPos(itr.m_tileBounds.left, itr.m_tileBounds.top);
				sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
				rect->setPosition(tempPos);
				rect->setFillColor(sf::Color(255, 255, 0, 150));
				m_entityManager->GetContext()->debugOverlay.Add(rect);
			}
			// End debug.
			float xDiff = (m_AABB.left + (m_AABB.width / 2)) - (itr.m_tileBounds.left + (itr.m_tileBounds.width / 2));
			float yDiff = (m_AABB.top + (m_AABB.height / 2)) - (itr.m_tileBounds.top + (itr.m_tileBounds.height / 2));
			float resolve = 0;
			if (abs(xDiff) > abs(yDiff))
			{
				if (xDiff > 0)
				{
					resolve = (itr.m_tileBounds.left + tileSize) - m_AABB.left;
				}
				else
				{
					resolve = -((m_AABB.left + m_AABB.width) - itr.m_tileBounds.left);
				}
				Move(resolve, 0);
				m_velocity.x = 0;
				m_collidingOnX = true;
			}
			else
			{
				if (yDiff > 0)
				{
					resolve = (itr.m_tileBounds.top + tileSize) - m_AABB.top;
				}
				else
				{
					resolve = -((m_AABB.top + m_AABB.height) - itr.m_tileBounds.top);
				}
				Move(0, resolve);
				m_velocity.y = 0;
				if (m_collidingOnY) { continue; }
				m_referenceTile = itr.m_tile;
				m_collidingOnY = true;
			}
		}
		m_collisions.clear();
	}
	if (!m_collidingOnY) { m_referenceTile = nullptr; }
}

CollisionElement::~CollisionElement()
{
	m_tile = nullptr;
}
