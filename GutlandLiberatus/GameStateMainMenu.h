#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>
#include <Kengine/Button.h>
#include <Kengine/ButtonContainer.h>

/*
   ===========================================================
   class GameStateMainMenu: public Kengine::BaseState
   A main menu where you can start/resume a game, change game
   settings, check out credits or/and exit the game.
   ===========================================================
 */
class GameStateMainMenu: public Kengine::BaseState
{
public:
    using BaseState::BaseState;
            GameStateMainMenu(StateManager * stateManager);
            ~GameStateMainMenu();

    void    OnCreate();
    void    OnDestroy();
    void    Activate();
    void    Deactivate();
    void    Update(const sf::Time& time);
    void    Draw();

    void    MouseClick(Kengine::EventDetails* details);
    void    KeyUp(Kengine::EventDetails* details);
    void    KeyDown(Kengine::EventDetails* details);
    void    KeyEnter(Kengine::EventDetails* details);

    void    checkButtons();

private:
    sf::Text                 m_text;
    sf::Font                 m_font;
    sf::Texture              m_textureBackground;
    sf::Sprite               m_spriteBackground;
    sf::Vector2f             m_buttonSize;
    sf::Vector2f             m_buttonPos;
    unsigned int             m_buttonPadding;

    Kengine::ButtonContainer m_buttons;
};
