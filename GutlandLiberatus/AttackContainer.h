#pragma once

#include <unordered_map>
#include <iostream>
#include <utility>

#include "Attack.h"

class AttackContainer {

public:
	AttackContainer::AttackContainer() {};
	AttackContainer::~AttackContainer() {};

	void addAttack(std::string name, unsigned int evasionModifier,
					unsigned int meleeHitModifier, unsigned int rangeHitModifier,
					unsigned int criticalchanceModifier);
	Attack getAttack(std::string name) const;

private:
	std::unordered_map<std::string, Attack>		m_container;

};