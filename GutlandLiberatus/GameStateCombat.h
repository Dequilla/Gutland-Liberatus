#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>
#include <Kengine/ButtonContainer.h>

#include "EnemyController.h"

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

private:
	Kengine::ButtonContainer m_buttons;

	sf::Texture m_portraitTexture;
	sf::Sprite m_portrait;

	sf::Texture m_portraitFrameTexture;
	sf::Sprite m_portraitFrame;

	sf::Texture m_bottomFrameTexture;
	sf::Sprite m_bottomFrame;

	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;

	sf::Font m_steamwreckFont;
	sf::Text m_healthText;
	sf::Text m_meleeHitChanceText;
	sf::Text m_rangeHitChanceText;
	sf::Text m_evasionText;

	EnemyController m_enemyController;
};
