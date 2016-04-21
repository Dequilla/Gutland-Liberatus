#include "CombatEnemy.h"

CombatEnemy::CombatEnemy()
{
	m_bodyTexture.loadFromFile("media/Textures/NPCDrunkard.png");
	m_bodySprite.setTexture(m_bodyTexture);
	m_bodySprite.setPosition(sf::Vector2f(500.f, 200.f));
	m_bodySprite.setScale(sf::Vector2f(15.f, 15.f));
	m_bodySprite.setOrigin(m_bodySprite.getLocalBounds().width / 2, m_bodySprite.getLocalBounds().height / 2);

	m_hightlightTexture.loadFromFile("media/Textures/NPCDrunkardHighlight.png");
	m_hightlightSprite.setTexture(m_hightlightTexture);
	m_hightlightSprite.setPosition(500.f, 200.f);
	m_hightlightSprite.setScale(sf::Vector2f(15.f, 15.f));

	m_hightlightSprite.setOrigin(m_hightlightSprite.getLocalBounds().width / 2, m_hightlightSprite.getLocalBounds().height / 2);
}

CombatEnemy::~CombatEnemy()
{
}

sf::Sprite CombatEnemy::getSprite()
{
	return m_bodySprite;
}

sf::Sprite CombatEnemy::getHightlightSprite()
{
	return m_hightlightSprite;
}
