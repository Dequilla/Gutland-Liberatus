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
	m_bodyTexture->loadFromFile(pathToBodySprite);
	m_bodySprite->setTexture(*m_bodyTexture);
	m_bodySprite->setPosition(position);
	m_bodySprite->setOrigin(m_bodySprite->getLocalBounds().width / 2, m_bodySprite->getLocalBounds().height / 2);
	m_bodySprite->setScale(sf::Vector2f(10.f, 10.f));

	m_highlightTexture->loadFromFile(pathToHighlightSprite);
	m_highlightSprite->setTexture(*m_highlightTexture);
	m_highlightSprite->setPosition(position);
	m_highlightSprite->setOrigin(m_highlightSprite->getLocalBounds().width / 2, m_highlightSprite->getLocalBounds().height / 2);
	m_highlightSprite->setScale(sf::Vector2f(10.f, 10.f));

	m_selectedSprite->setTexture(*m_highlightTexture);
	m_selectedSprite->setColor(sf::Color(230, 172, 0, 255));
	m_selectedSprite->setPosition(position);
	m_selectedSprite->setOrigin(m_highlightSprite->getLocalBounds().width / 2, m_highlightSprite->getLocalBounds().height / 2);
	m_selectedSprite->setScale(sf::Vector2f(10.f, 10.f));

	m_currentHealth = health;
	m_maxhealth = health;
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

bool CombatEnemy::contains(sf::Vector2i point)
{
	sf::FloatRect temp = m_bodySprite->getGlobalBounds();

	float sizeX = temp.width;
	float sizeY = temp.height;

	if (point.x >= m_bodySprite->getPosition().x - sizeX / 2 &&
		point.x <= m_bodySprite->getPosition().x + sizeX / 2 &&
		point.y >= m_bodySprite->getPosition().y - sizeY / 2 &&
		point.y <= m_bodySprite->getPosition().y + sizeY / 2)
	{
		return true;
	}
	return false;
}
