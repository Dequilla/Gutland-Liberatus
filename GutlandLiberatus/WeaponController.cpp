#include "WeaponController.h"

void WeaponController::addWeapon(std::string name, unsigned int rangeModifier, unsigned int meleeModifier, unsigned int evasionModifier, unsigned int maxDamage, unsigned int minimumDamage, unsigned int critChance, Weapon::Type type, std::string attacks[4])
{
	Weapon temp;

	temp.name = name;
	temp.rangeModifier = rangeModifier;
	temp.meleeModifier = meleeModifier;
	temp.evasionModifier = evasionModifier;
	temp.maxDamage = maxDamage;
	temp.minimumDamage = minimumDamage;
	temp.weaponType = type;
	temp.critChance = critChance;
	for (int i = 0; i < 4; i++)
	{
		temp.attacks[i] = attacks[i];
	}
	m_container.emplace(std::pair<std::string, Weapon>(name, temp));
}

Weapon WeaponController::getWeapon(std::string weaponName)
{
	for (auto &itr : m_container)
	{
		if (itr.first == weaponName)
		{
			return itr.second;
		}
	}
}
