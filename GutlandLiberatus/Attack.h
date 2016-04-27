#pragma once

#include <iostream>

struct Attack {

	unsigned int evasionModifier = 0;
	unsigned int meleeHitModifier = 0;
	unsigned int rangeHitModifier = 0;
	unsigned int critchanceModifier = 0;

	std::string name;

};