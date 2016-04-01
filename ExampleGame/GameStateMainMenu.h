#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/Button.h>

#include <Kengine/BaseState.h>

class GameStateMainMenu : public Kengine::BaseState
{
public:
	using BaseState::BaseState;
	~GameStateMainMenu();

	void OnCreate();
	void OnDestroy();
	void Activate();
	void Deactivate();
	void Update(const sf::Time& time);
	void Draw();

	void MouseClick(Kengine::EventDetails* details);
	void KeyUp(Kengine::EventDetails* details);
	void KeyDown(Kengine::EventDetails* details);
	void KeyEnter(Kengine::EventDetails* details);
private:
	sf::Text m_text;
	sf::Font m_font;
	sf::Texture m_textureBackground;
	sf::Sprite m_spriteBackground;

	Kengine::Button m_buttonStartGame;
	Kengine::Button m_buttonCredits;
	Kengine::Button m_buttonExitGame;
	Kengine::Button m_buttons[3] = { m_buttonStartGame, m_buttonCredits, m_buttonExitGame };

	int m_currentChoice = 0;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	sf::RectangleShape m_rects[3];
	sf::Text m_labels[3];
};