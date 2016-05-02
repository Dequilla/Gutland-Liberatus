#include "AttackContainer.h"

void AttackContainer::addAttack(std::string name, unsigned int evasionModifier, unsigned int meleeHitModifier, unsigned int rangeHitModifier, unsigned int criticalchanceModifier)
{
	Attack temp;
	temp.evasionModifier = evasionModifier;
	temp.meleeHitModifier = meleeHitModifier;
	temp.rangeHitModifier = rangeHitModifier;
	temp.critchanceModifier = criticalchanceModifier;
	temp.name = name;

	m_container.emplace(std::pair<std::string, Attack>(name, temp));
}

Attack AttackContainer::getAttack(std::string name) const
{
	for (auto &itr : m_container)
	{
		if (itr.first == name)
		{
			return itr.second;
		}
	}
}
