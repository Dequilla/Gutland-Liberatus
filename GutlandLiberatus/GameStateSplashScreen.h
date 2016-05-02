#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>
#include <Kengine/TextureManager.h>
#include <Kengine/SpriteSheet.h>

class GameStateSplashScreen: public Kengine::BaseState
{
public:
            GameStateSplashScreen(StateManager * statemanager);
            ~GameStateSplashScreen();

    void    OnCreate();
    void    OnDestroy();
    void    Activate();
    void    Deactivate();
    void    Update(const sf::Time& time);
    void    Draw();

    void    Continue(Kengine::EventDetails* details = nullptr);

private:
    sf::Texture          m_texture;
    sf::Sprite           m_sprite;

    Kengine::SpriteSheet *m_spriteSheet;

    int                  m_opacity;
    float                m_elapsedTime;
    float                m_opacityChecker;
    bool                 m_fading            = false;
    bool                 m_finished          = false;
    bool                 m_secondSplashCheck = false;
    int                  m_splashScreenIndex = 0;
};
