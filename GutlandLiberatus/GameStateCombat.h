#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>
#include <Kengine/ButtonContainer.h>

#include "EnemyController.h"
#include "CharacterContext.h"
#include "AttackContainer.h"
#include "WeaponController.h"

#include "Map.h"

class GameStateCombat : public Kengine::BaseState
{
public:
	GameStateCombat(StateManager* stateManager);
	~GameStateCombat();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void MainMenu(Kengine::EventDetails* details);
	void Pause(Kengine::EventDetails* details);
	void ToggleOverlay(Kengine::EventDetails* details);
	void MouseClick(Kengine::EventDetails* details);

	static inline std::string int2Str(int x)
	{
		std::stringstream type;
		type << x;
		return type.str();
	}
private:
	Kengine::ButtonContainer	m_buttons;
	Kengine::ButtonContainer	m_extraButtons;

	sf::Texture					m_portraitTexture;
	sf::Sprite					m_portrait;

	sf::Texture					m_portraitFrameTexture;
	sf::Sprite					m_portraitFrame;

	sf::Texture					m_bottomFrameTexture;
	sf::Sprite					m_bottomFrame;

	sf::Texture					m_backgroundTexture;
	sf::Sprite					m_background;

	sf::Font					m_steamwreckFont;
	sf::Text					m_healthText;
	sf::Text					m_meleeHitChanceText;
	sf::Text					m_rangeHitChanceText;
	sf::Text					m_evasionText;

	EnemyController				m_enemyController;
	CharacterContext			m_characterContext;
	AttackContainer				m_attacks;
	WeaponController			m_weapons;

	std::string					m_buttonMode = "";
	bool						m_playersTurn = true;
	bool						m_showExtraButtons = false;
};
