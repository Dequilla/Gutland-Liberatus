#include "CombatEnemy.h"

CombatEnemy::CombatEnemy()
{

}

CombatEnemy::~CombatEnemy()
{
	//delete m_bodySprite;
	//delete m_highlightSprite;
}

void CombatEnemy::create(std::string pathToBodySprite, std::string pathToHighlightSprite, unsigned int health, sf::Vector2f position, unsigned int range = 1)
{
	sf::Vector2f scaleModifier;
	sf::Vector2f positionModifier;

	if (range == 1)
	{
		scaleModifier = sf::Vector2f(4.f, 4.f);
		positionModifier = sf::Vector2f(0.f, 0.f);
	}
	else if (range == 2)
	{
		scaleModifier = sf::Vector2f(1.f, 1.f);
		positionModifier = sf::Vector2f(0.f, -100.f);
	}
	else if (range == 3)
	{
		scaleModifier = sf::Vector2f(-2.f, -2.f);
		positionModifier = sf::Vector2f(0.f, -200.f);
	}

	m_bodyTexture->loadFromFile(pathToBodySprite);
	m_bodySprite->setTexture(*m_bodyTexture);
	m_bodySprite->setPosition(position + positionModifier);
	m_bodySprite->setOrigin(m_bodySprite->getLocalBounds().width / 2, m_bodySprite->getLocalBounds().height / 2);
	m_bodySprite->setScale(sf::Vector2f(10.f, 10.f) + scaleModifier);

	m_highlightTexture->loadFromFile(pathToHighlightSprite);
	m_highlightSprite->setTexture(*m_highlightTexture);
	m_highlightSprite->setPosition(position + positionModifier);
	m_highlightSprite->setOrigin(m_highlightSprite->getLocalBounds().width / 2, m_highlightSprite->getLocalBounds().height / 2);
	m_highlightSprite->setScale(sf::Vector2f(10.f, 10.f) + scaleModifier);

	m_selectedSprite->setTexture(*m_highlightTexture);
	m_selectedSprite->setColor(sf::Color(230, 172, 0, 255));
	m_selectedSprite->setPosition(position + positionModifier);
	m_selectedSprite->setOrigin(m_highlightSprite->getLocalBounds().width / 2, m_highlightSprite->getLocalBounds().height / 2);
	m_selectedSprite->setScale(sf::Vector2f(10.f, 10.f) + scaleModifier);

	m_details.setCurrentHealth(health);
	m_details.setHealthModifier(0);
	m_details.setMaxHealth(health);
	m_details.setEvasion(10);
	m_details.setEvasionModifier(0);
	m_details.setMeleeHitChance(70);
	m_details.setMeleeHitChanceModifier(0);
	m_details.setRangeHitChance(0);
	m_details.setRangeHitChanceModifier(0);
	m_range = range;
}

void CombatEnemy::draw(sf::RenderWindow * window)
{
	window->draw(*m_bodySprite);
	if (m_active)
	{
		window->draw(*m_highlightSprite);
	}
	if (m_selected)
	{
		window->draw(*m_selectedSprite);
	}
}

void CombatEnemy::update(float dt)
{
	//m_bodySprite->setColor(sf::Color(255, 255 / (m_details.getCurrentHealth() * 0.01), 255 / (m_details.getCurrentHealth() * 0.01), 255));
	//m_details.setCurrentHealth(m_details.getCurrentHealth() - 1);
}

void CombatEnemy::setActive(bool tof)
{
	m_active = tof;
}

bool CombatEnemy::isActive()
{
	return m_active;
}

void CombatEnemy::setSelected(bool tof)
{
	m_selected = tof;
}

bool CombatEnemy::isSelected()
{
	return m_selected;
}

bool CombatEnemy::contains(sf::Vector2i point)
{
	sf::FloatRect temp = m_bodySprite->getGlobalBounds();

	float sizeX = temp.width;
	float sizeY = temp.height;

	if (point.x >= m_bodySprite->getPosition().x - (sizeX / 2) + 50 &&
		point.x <= m_bodySprite->getPosition().x + (sizeX / 2) - 50 &&
		point.y >= m_bodySprite->getPosition().y - (sizeY / 2) + 50 &&
		point.y <= m_bodySprite->getPosition().y + (sizeY / 2))
	{
		return true;
	}
	return false;
}

CharacterContext * CombatEnemy::getCharacterContext()
{
	return &m_details;
}

void CombatEnemy::updateColor()
{
	float modifier = (m_details.getCurrentHealth() * 0.01) * 255;
	std::cout << "Enemy color modifier:"  << modifier << std::endl;
	m_bodySprite->setColor(sf::Color(255, modifier, modifier, 255));
}

void CombatEnemy::setDead(bool tof)
{
	m_dead = tof;
	if (tof = true)
	{
		m_selected = false;
	}
}

bool CombatEnemy::isDead()
{
	return m_dead;
}
