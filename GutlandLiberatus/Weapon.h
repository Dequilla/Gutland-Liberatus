#pragma once
#include <vector>

struct Weapon {
	Weapon::Weapon(){};
	Weapon::~Weapon(){};

	unsigned int rangeModifier = 0;
	unsigned int meleeModifier = 0;
	unsigned int evasionModifier = 0;
	unsigned int maxDamage = 0;
	unsigned int minimumDamage = 0;

	std::string					name;
	std::vector<std::string>	attacks;

};