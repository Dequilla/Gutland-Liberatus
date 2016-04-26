#pragma once
#include <unordered_map>
#include <time.h>
#include <stdlib.h>
#include <iostream>

#include "CombatEnemy.h"

class EnemyController {
	public:
		EnemyController::EnemyController();
		EnemyController::~EnemyController();

		void draw(sf::RenderWindow* window);

		void generateNewRandomEnemies();
		void update(sf::Vector2i mousePos, sf::RenderWindow* window);

		void changeSelected();

		void createEnemies(std::string enemies[3]);

		void resetCombat();

	private:
		std::unordered_map<int, CombatEnemy>	m_enemyContainer;

		std::string								m_enemiesAllowedToBeGenerated[2] = {"Drunk", "Hunter"};

		sf::RenderWindow*						m_window;
		sf::Vector2i							m_mousePos;
};