#include "AttackContainer.h"

void AttackContainer::addAttack(std::string name, Attack attack)
{
	m_container.emplace(name, attack);
}

Attack AttackContainer::getAttack(std::string name)
{
	for (auto &itr : m_container)
	{
		if (itr.first == name)
		{
			return itr.second;
		}
	}
}
