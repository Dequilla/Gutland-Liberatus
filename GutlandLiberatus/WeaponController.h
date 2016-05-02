#pragma once
#include <unordered_map>
#include <utility>

#include "Weapon.h"


class WeaponController {
	
public:
	WeaponController::WeaponController() {};
	WeaponController::~WeaponController() {};

	void addWeapon(std::string name, unsigned int rangeModifier,
					unsigned int meleeModifier, unsigned int evasionModifier,
					unsigned int maxDamage, unsigned int minimumDamage,
					unsigned int critChance,
					Weapon::Type type, std::string attacks[4]);

	Weapon getWeapon(std::string weaponName);

private:
	std::unordered_map<std::string, Weapon>		m_container;


};