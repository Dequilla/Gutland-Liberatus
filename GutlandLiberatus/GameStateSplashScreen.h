#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>

class GameStateSplashScreen : public Kengine::BaseState
{
public:
	using BaseState::BaseState;
	~GameStateSplashScreen();

	void OnCreate();
	void OnDestroy();
	void Activate();
	void Deactivate();
	void Update(const sf::Time& time);
	void Draw();

	void Continue(Kengine::EventDetails* details);

private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	int m_opacity;
	float m_elapsedTime;
	float m_opacityChecker;
	bool m_fading = false;
	bool m_finished = false;
};