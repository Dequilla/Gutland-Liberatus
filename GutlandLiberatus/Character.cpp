#include "Character.h"
#include "EntityManager.h"
#include "StateManager.h"

Character::Character(EntityManager* entityManager) :
	EntityBase(entityManager), m_spriteSheet(m_entityManager->GetContext()->textureManager),
	m_jumpVelocity(250.0f), m_hitpoints(5)
{
	m_name = "Character";
}

Character::~Character()
{
	m_spriteSheet.ReleaseSheet();
}

void Character::Move(const Kengine::Direction& dir)
{
	if (GetState() == EntityState::Dying) { return; }
	m_spriteSheet.SetDirection(dir);
	if (dir == Kengine::Direction::Left) { Accelerate(-m_speed.x, 0); }
	else if (dir == Kengine::Direction::Right) { Accelerate(m_speed.x, 0); }
	else if (dir == Kengine::Direction::Up) { Accelerate(0, -m_speed.y); }
	else if (dir == Kengine::Direction::Down) { Accelerate(0, m_speed.y); }
	if (GetState() == EntityState::Idle) { SetState(EntityState::Walking); }

}

void Character::Jump()
{
	if (GetState() == EntityState::Dying || 
		GetState() == EntityState::Jumping || 
		GetState() == EntityState::Hurt) { return; }

	SetState(EntityState::Jumping);
	AddVelocity(0, -m_jumpVelocity);
}

void Character::Attack()
{
	if (GetState() == EntityState::Dying || 
		GetState() == EntityState::Jumping ||
		GetState() == EntityState::Hurt || 
		GetState() == EntityState::Attacking)
	{
		return;
	}

	SetState(EntityState::Attacking);
}

void Character::GetHurt(const int& damage)
{
	if (GetState() == EntityState::Dying || 
		GetState() == EntityState::Hurt) { return; }

	m_hitpoints = (m_hitpoints - damage > 0 ? m_hitpoints - damage : 0);

	if (m_hitpoints) { SetState(EntityState::Hurt); }
	else { SetState(EntityState::Dying); }
}

void Character::Load(const std::string& path)
{
	std::ifstream file;
	file.open(std::string("media/Characters/") + path);
	if (!file.is_open()) { std::cout << "! Failed loading the character file: " << path << std::endl; return; }
	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|') { continue; }
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "Name")
		{
			keystream >> m_name;
		}
		else if (type == "Spritesheet")
		{
			std::string path;
			keystream >> path;
			m_spriteSheet.LoadSheet("media/SpriteSheets/" + path);
		}
		else if (type == "Hitpoints")
		{
			keystream >> m_hitpoints;
		}
		else if (type == "BoundingBox")
		{
			sf::Vector2f boundingSize;
			keystream >> boundingSize.x >> boundingSize.y;
			SetSize(boundingSize.x, boundingSize.y);
		}
		else if (type == "DamageBox")
		{
			keystream >> m_attackAABBoffset.x >> m_attackAABBoffset.y
				>> m_attackAABB.width >> m_attackAABB.height;
		}
		else if (type == "Speed")
		{
			keystream >> m_speed.x >> m_speed.y;
		}
		else if (type == "JumpVelocity")
		{
			keystream >> m_jumpVelocity;
		}
		else if (type == "MaxVelocity")
		{
			keystream >> m_maxVelocity.x >> m_maxVelocity.y;
		}
		else
		{
			std::cout << "! Unknown type in character file: " << type << std::endl;
		}
	}
	file.close();
}

void Character::Update(float dt)
{
	EntityBase::Update(dt);
	if (m_attackAABB.width != 0 && m_attackAABB.height != 0)
	{
		UpdateAttackAABB();

		// Debug.
		if (m_entityManager->GetContext()->debugOverlay.Debug())
		{
			sf::RectangleShape* arect = new sf::RectangleShape(sf::Vector2f(m_attackAABB.width, m_attackAABB.height));
			arect->setPosition(m_attackAABB.left, m_attackAABB.top);
			arect->setFillColor(sf::Color(255, 0, 0,
										  (m_state == EntityState::Attacking && m_spriteSheet.GetCurrentAnimation()->IsInAction()
										   ? 200 : 100)));
			m_entityManager->GetContext()->debugOverlay.Add(arect);

			sf::RectangleShape* boundingRect = new sf::RectangleShape(sf::Vector2f(GetAABB().width, GetAABB().height));
			boundingRect->setPosition(GetAABB().left, GetAABB().top);
			boundingRect->setFillColor(sf::Color(255, 255, 255, 200));
			m_entityManager->GetContext()->debugOverlay.Add(boundingRect);

		}
		// End debug.
	}
	if (GetState() != EntityState::Dying && GetState() != EntityState::Attacking && GetState() != EntityState::Hurt)
	{
		if (abs(m_velocity.y) >= 0.01f)
		{
			SetState(EntityState::Walking);
		}
		else if (abs(m_velocity.x) >= 0.1f)
		{
			SetState(EntityState::Walking);
		}
		else
		{
			SetState(EntityState::Idle);
		}
	}
	else if (GetState() == EntityState::Attacking || GetState() == EntityState::Hurt)
	{
		if (!m_spriteSheet.GetCurrentAnimation()->IsPlaying())
		{
			SetState(EntityState::Idle);
		}
	}
	else if (GetState() == EntityState::Dying)
	{
		if (!m_spriteSheet.GetCurrentAnimation()->IsPlaying())
		{
			m_entityManager->Remove(m_id);
		}
	}

	Animate();

	m_spriteSheet.Update(dt);
	m_spriteSheet.SetSpritePosition(m_position);
}

void Character::Draw(sf::RenderWindow* window)
{
	m_spriteSheet.Draw(window);
}

void Character::UpdateAttackAABB()
{
	if (m_spriteSheet.GetDirection() != Kengine::Direction::Up &&
		m_spriteSheet.GetDirection() != Kengine::Direction::Down)
	{
		m_attackAABB.left = (m_spriteSheet.GetDirection() == Kengine::Direction::Left ?
							 (m_AABB.left - m_attackAABB.width) - m_attackAABBoffset.x : 
							 (m_AABB.left + m_AABB.width) + m_attackAABBoffset.x);
		m_attackAABB.top = m_AABB.top + m_attackAABBoffset.y;
	}
	else
	{
		m_attackAABB.top = (m_spriteSheet.GetDirection() == Kengine::Direction::Up ?
							(m_AABB.top - m_attackAABB.height) - m_attackAABBoffset.y :
							(m_AABB.top + m_attackAABB.height) + m_attackAABBoffset.y);
		m_attackAABB.left = m_AABB.left + m_attackAABBoffset.x;
	}
}

void Character::Animate()
{
	EntityState state = GetState();

	if (state == EntityState::Walking && m_spriteSheet.
		GetCurrentAnimation()->GetName() != "Walk")
	{
		m_spriteSheet.SetAnimation("Walk", true, true);
	}
	else if (state == EntityState::Jumping && m_spriteSheet.
			 GetCurrentAnimation()->GetName() != "Jump")
	{
		m_spriteSheet.SetAnimation("Jump", true, false);
	}
	else if (state == EntityState::Attacking && m_spriteSheet.
			 GetCurrentAnimation()->GetName() != "Attack")
	{
		m_spriteSheet.SetAnimation("Attack", true, false);
	}
	else if (state == EntityState::Hurt && m_spriteSheet.
			 GetCurrentAnimation()->GetName() != "Hurt")
	{
		m_spriteSheet.SetAnimation("Hurt", true, false);
	}
	else if (state == EntityState::Dying && m_spriteSheet.
			 GetCurrentAnimation()->GetName() != "Death")
	{
		m_spriteSheet.SetAnimation("Death", true, false);
	}
	else if (state == EntityState::Idle && m_spriteSheet.
			 GetCurrentAnimation()->GetName() != "Idle")
	{
		if (m_spriteSheet.GetDirection() == Kengine::Direction::Up)
		{
			m_spriteSheet.SetAnimation("Idle", false, false);
		}
		else
		{
			m_spriteSheet.SetAnimation("Idle", false, false);
		}
	}
}
