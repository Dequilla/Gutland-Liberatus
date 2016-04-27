#pragma once

#include <unordered_map>

#include "Attack.h"

class AttackContainer {

public:
	AttackContainer::AttackContainer() {};
	AttackContainer::~AttackContainer() {};

	void addAttack(std::string name, Attack attack);
	Attack getAttack(std::string name);

private:
	std::unordered_map<std::string, Attack>		m_container;

};