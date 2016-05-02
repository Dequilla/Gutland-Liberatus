#pragma once

#include <Kengine/Window.h>
#include <Kengine/EventManager.h>
#include <Kengine/TextureManager.h>

#include <Kengine/Kopter.h>

#include "StateManager.h"
#include "EntityManager.h"

#include <iostream>

/*
   ===========================================================
   This is the core of the game.
   ===========================================================
 */
class Game
{
public:
                            Game();
                            ~Game();

    void                    Update();
    void                    LateUpdate();
    void                    Render();

    Kengine::Window*        GetWindow();
    sf::Time                GetElapsed();

private:
    Kengine::Window         m_window;
    sf::Clock               m_clock;
    sf::Time                m_elapsed;
    Kengine::Kopter         m_options;
    EntityManager           m_entityManager;
    StateManager            m_stateManager;
    SharedContext           m_context;
    Kengine::TextureManager m_textureManager;

    void                    RestartClock();
};
