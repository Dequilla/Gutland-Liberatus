#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>
#include <Kengine/ButtonContainer.h>
#include <Kengine/Kopter.h>

#include "Map.h"

class GameStateOptionsMenu : public Kengine::BaseState
{
public:
	GameStateOptionsMenu(StateManager* stateManager);
	~GameStateOptionsMenu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();
	
	void Update(const sf::Time& time);
	void Draw();

	void MainMenu(Kengine::EventDetails* details);
	void ToggleOverlay(Kengine::EventDetails* details);

	void checkButtons(Kengine::EventDetails* details);

private:
	void MouseClick(Kengine::EventDetails* details = nullptr);

	Kengine::Kopter* m_options;
	Kengine::ButtonContainer m_buttons;
	sf::Texture m_textureBackground;
	sf::Sprite m_spriteBackground;
	sf::Font m_font;
	sf::Text m_text;
	sf::Text m_fullscreenText;
	sf::Text m_verticalsyncText;


};