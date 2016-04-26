#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class CombatEnemy {

public:
	CombatEnemy::CombatEnemy();
	CombatEnemy::~CombatEnemy();

	void create(std::string pathToBodySprite, std::string pathToHighlightSprite, unsigned int health,
				sf::Vector2f position, unsigned int range);

	void draw(sf::RenderWindow* window);

private:
	sf::Texture* m_bodyTexture = new sf::Texture;
	sf::Sprite* m_bodySprite = new sf::Sprite;

	sf::Texture* m_highlightTexture = new sf::Texture;
	sf::Sprite* m_highlightSprite = new sf::Sprite;

	unsigned int m_currentHealth = 0;
	unsigned int m_maxhealth = 0;

	bool m_active = false;
};