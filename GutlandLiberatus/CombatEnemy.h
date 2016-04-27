#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "CharacterContext.h"

class CombatEnemy {

public:
	CombatEnemy::CombatEnemy();
	CombatEnemy::~CombatEnemy();

	void create(std::string pathToBodySprite, std::string pathToHighlightSprite, unsigned int health,
				sf::Vector2f position, unsigned int range);

	void draw(sf::RenderWindow* window);
	void update(float dt);

	void setActive(bool tof);
	bool isActive();
	void setSelected(bool tof);
	bool isSelected();
	bool contains(sf::Vector2i point);

private:
	sf::Texture*		m_bodyTexture = new sf::Texture;
	sf::Sprite*			m_bodySprite = new sf::Sprite;

	sf::Texture*		m_highlightTexture = new sf::Texture;
	sf::Sprite*			m_highlightSprite = new sf::Sprite;

	sf::Sprite*			m_selectedSprite = new sf::Sprite;

	CharacterContext	m_details;

	unsigned int		m_range = 1;

	bool				m_active = false;
	bool				m_selected = false;
};