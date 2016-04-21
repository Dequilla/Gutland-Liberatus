#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>
#include <Kengine/ButtonContainer.h>

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

	sf::Sprite m_portraitFrame;
	sf::Sprite m_bottomFrame;
	sf::Texture m_portraitFrameTexture;
	sf::Texture m_bottomFrameTexture;

	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
};
