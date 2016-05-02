#include "CharacterContext.h"

CharacterContext::CharacterContext()
{
}

CharacterContext::~CharacterContext()
{
}

void CharacterContext::dealDamage(unsigned int damage)
{
	m_currentHealth -= damage;
}
