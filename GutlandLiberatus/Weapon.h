#pragma once
#include <vector>

struct Weapon {
	enum class Type {
		Melee = 0,
		Range
	};

	Weapon::Weapon(){};
	Weapon::~Weapon(){};

	unsigned int rangeModifier = 0;
	unsigned int meleeModifier = 0;
	unsigned int evasionModifier = 0;
	unsigned int maxDamage = 0;
	unsigned int minimumDamage = 0;
	unsigned int critChance = 0;

	std::string					name;
	Type						weaponType;
	std::string					attacks[4];

};