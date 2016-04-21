#pragma once

#include <SFML/Graphics.hpp>

class CombatEnemy {

public:
	CombatEnemy::CombatEnemy();
	CombatEnemy::~CombatEnemy();

	sf::Sprite getSprite();
	sf::Sprite getHightlightSprite();

private:
	sf::Texture m_bodyTexture;
	sf::Sprite m_bodySprite;

	sf::Texture m_hightlightTexture;
	sf::Sprite m_hightlightSprite;
};