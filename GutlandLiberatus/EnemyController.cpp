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
		if (!itr.second.isDead())
		{
			itr.second.draw(window);
		}
	}
}

void EnemyController::generateNewRandomEnemies()
{
	//Code that generates 1-3 new randomly selected enemies
	unsigned int enemyAmount = rand() % 3 + 1;

	unsigned int amountOfAllowedEnemies = (sizeof(m_enemiesAllowedToBeGenerated) / sizeof(*m_enemiesAllowedToBeGenerated));

	m_numberOfEnemies = enemyAmount;
	m_numberOfEnemiesStatic = enemyAmount;

	std::string enemies[3];
	if (enemyAmount > 3)
	{
		std::cout << "This impossible!!!" << std::endl;
		return;
	}
	if (enemyAmount > 0)
	{ 
		enemies[0] = m_enemiesAllowedToBeGenerated[ rand() % amountOfAllowedEnemies ];
	}
	if (enemyAmount > 1)
	{
		enemies[1] = m_enemiesAllowedToBeGenerated[rand() % amountOfAllowedEnemies ];
	}
	if (enemyAmount > 2) 
	{
		enemies[2] = m_enemiesAllowedToBeGenerated[ rand() % amountOfAllowedEnemies ];
	}

	createEnemies(enemies);
}

int EnemyController::getCurrentAmountOfEnemies()
{
	return m_numberOfEnemies;
}

int EnemyController::getCurrentAmountOfDeadEnemies()
{
	return m_numberOfDeadsCurrently;
}

int EnemyController::getAmountOfEnemies()
{
	return m_numberOfEnemiesStatic;
}

void EnemyController::update(sf::Vector2i mousePos, sf::RenderWindow * window)
{
	m_window = window;
	m_mousePos = mousePos;

	//Check buttons
	for (auto &itr : m_enemyContainer)
	{
		if(!itr.second.isDead())
		{
			if (itr.second.contains(m_mousePos)) itr.second.setActive(true);
			else itr.second.setActive(false);

			itr.second.update(60);
		}
	}
}

void EnemyController::changeSelected()
{
	int oneIsActive = 0;
	int twoIsSelected = 0;
	for (auto &itr : m_enemyContainer)
	{
		if (itr.second.isActive()) oneIsActive++;
	}
	if (oneIsActive <= 0) return;

	for (auto &itr : m_enemyContainer)
	{
		if (itr.second.isActive())
		{
			itr.second.setSelected(true);
		}
		else if(!itr.second.isActive())
		{
			itr.second.setSelected(false);
		}
	}

	for (auto &itr : m_enemyContainer)
	{
		if (itr.second.isSelected()) twoIsSelected++;
		if (twoIsSelected > 1)
		{
			for (auto &itr : m_enemyContainer)
			{
				itr.second.setSelected(false);
			}
		}
	}
}

CombatEnemy * EnemyController::getSelectedEnemy()
{
	for (auto &itr : m_enemyContainer)
	{
		if (itr.second.isSelected())
		{
			return &itr.second;
		}
	}
	return nullptr;
}

void EnemyController::createEnemies(std::string enemies[3])
{
	for (unsigned int i = 0; i < 3; i++)
	{
		unsigned int position;
		if (i == 0) position = 640;
		else if (i == 1) position = 320;
		else if (i == 2) position = 960;

		int range = rand() % 3 + 1;
		
		if (enemies[i] == "Drunk")
		{
			//CREATE DRUNK and place in container
			CombatEnemy temp;
			temp.create("media/Textures/NPC/" + enemies[i] + ".png", "media/Textures/NPC/" + enemies[i] + "Highlight.png", 100, sf::Vector2f(position, 300), range);
			m_enemyContainer.emplace(i, temp);
		}
		else if (enemies[i] == "Hunter")
		{
			//CREATE HUNTER and place in container
			CombatEnemy temp;
			temp.create("media/Textures/NPC/" + enemies[i] + ".png", "media/Textures/NPC/" + enemies[i] + "Highlight.png", 100, sf::Vector2f(position, 300), range);
			m_enemyContainer.emplace(i, temp);
		}
	}
}

bool EnemyController::checkCombat()
{
	int numberOfDeads = 0;
	for (auto &itr : m_enemyContainer)
	{
		if (itr.second.isDead())
		{
			numberOfDeads++;
			std::cout << "Number if dead people is = " << numberOfDeads << std::endl;
		}
	}
	m_numberOfDeadsCurrently = numberOfDeads;
	m_numberOfEnemies = m_numberOfEnemiesStatic - m_numberOfDeadsCurrently;
	if (numberOfDeads == m_numberOfEnemiesStatic)
	{
		std::cout << "Resetting with m_numberOfEnemies = " << m_numberOfEnemies << std::endl;
		resetCombat();
		std::cout << "YOU WON" << std::endl;
		return true;
	}
	return false;
}

void EnemyController::resetCombat()
{
	m_enemyContainer.clear();
}
