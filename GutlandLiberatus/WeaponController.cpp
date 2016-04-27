#include "WeaponController.h"

void WeaponController::addWeapon(std::string name, Weapon& weapon)
{
	m_container.emplace(name, weapon);
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
	return;
}
