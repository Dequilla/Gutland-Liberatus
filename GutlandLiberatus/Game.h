#pragma once

#include <Kengine/Window.h>
#include <Kengine/EventManager.h>
#include <Kengine/TextureManager.h>

#include "StateManager.h"
#include "EntityManager.h"

#include <iostream>

class Game
{
public:
    Game();
    ~Game();

    void Update();
    void LateUpdate();
    void Render();

    Kengine::Window* GetWindow() { return &m_window; }

    // Timing
    sf::Time GetElapsed() { return m_elapsed; }

    private:
    Kengine::Window m_window;
    sf::Clock m_clock;
    sf::Time m_elapsed;
    
	EntityManager m_entityManager;
    StateManager m_stateManager;
    SharedContext m_context;
	Kengine::TextureManager m_textureManager;

	void RestartClock() { m_elapsed = m_clock.restart(); }
};