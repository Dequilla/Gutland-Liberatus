#pragma once
#include <unordered_map>

#include "Weapon.h"

class WeaponController {
	
public:
	WeaponController::WeaponController() {};
	WeaponController::~WeaponController() {};

	void addWeapon(std::string name, Weapon& weapon);

	Weapon getWeapon(std::string weaponName);

private:
	std::unordered_map<std::string, Weapon>		m_container;


};