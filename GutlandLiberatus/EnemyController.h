#pragma once
#include <unordered_map>

#include "CombatEnemy.h"

class EnemyController {
	public:
		EnemyController::EnemyController();
		EnemyController::~EnemyController();

		void generateNewEnemies();

	private:
		std::unordered_map<int, CombatEnemy> m_enemyContainer;
};