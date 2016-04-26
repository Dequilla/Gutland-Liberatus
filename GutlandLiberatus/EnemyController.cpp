#include "EnemyController.h"

EnemyController::EnemyController()
{
	srand(time(NULL));
}

EnemyController::~EnemyController()
{

}

void EnemyController::draw(sf::RenderWindow* window)
{
	for (auto &itr : m_enemyContainer) 
	{
		itr.second.draw(window);
	}
}

void EnemyController::generateNewRandomEnemies()
{
	//Code that generates 1-3 new randomly selected enemies
	unsigned int enemyAmount = rand() % 3 + 1;

	std::string enemies[3];
	if (enemyAmount > 3)
	{
		std::cout << "This impossible!!!" << std::endl;
		return;
	}
	if (enemyAmount > 0)
	{ 
		enemies[0] = m_enemiesAllowedToBeGenerated[rand() % 2];
	}
	if (enemyAmount > 1)
	{
		enemies[1] = m_enemiesAllowedToBeGenerated[rand() % 2];
	}
	if (enemyAmount > 2) 
	{
		enemies[2] = m_enemiesAllowedToBeGenerated[rand() % 2];
	}

	createEnemies(enemies);
}

void EnemyController::createEnemies(std::string enemies[3])
{
	for (unsigned int i = 0; i < 3; i++)
	{
		unsigned int position;
		if (i == 0) position = 640;
		else if (i == 1) position = 320;
		else if (i == 2) position = 960;
		
		if (enemies[i] == "Drunk")
		{
			//CREATE DRUNK and place in container
			CombatEnemy temp;
			temp.create("media/Textures/NPCDrunkard.png", "media/Textures/NPCDrunkardHighlight.png", 100, sf::Vector2f(position, 200), 1);
			m_enemyContainer.emplace(i, temp);
		}
		else if (enemies[i] == "Hunter")
		{
			//CREATE HUNTER and place in container
			CombatEnemy temp;
			temp.create("media/Textures/NPCHunter.png", "media/Textures/NPCHunterHighlight.png", 100, sf::Vector2f(position, 200), 1);
			m_enemyContainer.emplace(i, temp);
		}
	}
}
