#pragma once
#include "Weapon.h"

class CharacterContext
{
public:
	CharacterContext::CharacterContext();
	CharacterContext::~CharacterContext(); 

	void setMaxHealth(unsigned int maxHealth) { m_maxHealth = maxHealth; }
	void setCurrentHealth(unsigned int health) { m_currentHealth = health; }
	void setHealthModifier(unsigned int healthModifier) { m_maxHealthModifier = healthModifier; }

	void setMeleeHitChance(unsigned int hitChance) { m_meleeHitChance = hitChance; }
	void setMeleeHitChanceModifier(unsigned int meleeHitChanceModifier) { m_meleeHitChanceModifier = meleeHitChanceModifier; }

	void setRangeHitChance(unsigned int hitChance) { m_rangeHitChance = hitChance; }
	void setRangeHitChanceModifier(unsigned int hitChance) { m_rangeHitChanceModifier = hitChance; }

	void setEvasion(unsigned int evasion) { m_evasion = evasion; }
	void setEvasionModifier(unsigned int evasion) { m_evasionModifier = evasion; }

	unsigned int getMaxHealth() { return m_maxHealth; }
	unsigned int getCurrentHealth() { return m_currentHealth; }
	unsigned int getHealthModifier() { return m_maxHealthModifier; }

	unsigned int getMeleeHitChance() { return m_meleeHitChance; }
	unsigned int getMeleeHitChanceModifier() { return m_meleeHitChanceModifier; }

	unsigned int getRangeHitChance() { return m_rangeHitChance; }
	unsigned int getRangeHitChanceModifier() { return m_rangeHitChanceModifier; }

	unsigned int getEvasion() { return m_evasion; }
	unsigned int getEvasionModifier() { return m_evasionModifier; }

	void setWeapon(Weapon weapon) { m_weapon = weapon; }
	Weapon getWeapon() { return m_weapon; }

private:
	unsigned int m_maxHealth = 100;
	unsigned int m_maxHealthModifier = 0;
	unsigned int m_currentHealth = 100;

	unsigned int m_meleeHitChance = 90;
	unsigned int m_meleeHitChanceModifier = 0;

	unsigned int m_rangeHitChance = 0;
	unsigned int m_rangeHitChanceModifier = 0;

	unsigned int m_evasion = 10;
	unsigned int m_evasionModifier = 0;

	Weapon m_weapon;

};