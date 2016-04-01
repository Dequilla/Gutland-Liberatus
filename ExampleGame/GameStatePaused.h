#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>

class GameStatePaused : public Kengine::BaseState
{
public:
	using BaseState::BaseState;
	~GameStatePaused();

	void OnCreate();
	void OnDestroy();
	void Activate();
	void Deactivate();
	void Update(const sf::Time& time);
	void Draw();

	void Unpause(Kengine::EventDetails* details);

private:
	sf::Text m_text;
	sf::RectangleShape m_rect;

	sf::Font m_font;
};