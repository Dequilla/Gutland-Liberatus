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
		int getCurrentAmountOfEnemies();
		int getAmountOFEnemies();
		void update(sf::Vector2i mousePos, sf::RenderWindow* window);

		void changeSelected();
		CombatEnemy* getSelectedEnemy();

		void createEnemies(std::string enemies[3]);

		void checkCombat();

		void resetCombat();

	private:
		std::unordered_map<int, CombatEnemy>	m_enemyContainer;

		std::string								m_enemiesAllowedToBeGenerated[2] = {"Drunk", "Hunter"};

		sf::RenderWindow*						m_window;
		sf::Vector2i							m_mousePos;
		
		int										m_numberOfEnemies = 0;
		int										m_numberOfEnemiesStatic = 0;
};