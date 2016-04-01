#pragma once

#include <SFML/Graphics.hpp>
#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>

class GameStateIntro : public Kengine::BaseState
{
public:
	using BaseState::BaseState;
    ~GameStateIntro();
    
    void OnCreate();
    void OnDestroy();
    void Activate();
    void Deactivate();
    void Update(const sf::Time& time);
    void Draw();
    
    void Continue(Kengine::EventDetails* details);
    
    private:
    sf::Texture m_introTexture;
    sf::Sprite m_introSprite;
    sf::Font m_font;
    sf::Text m_text;
    
    float m_timePassed;
};