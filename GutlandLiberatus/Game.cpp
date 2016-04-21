#include "Game.h"

Game::Game() : m_window("Gutland Liberatus", sf::Vector2u(1280, 720)),
m_stateManager(&m_context), m_entityManager(&m_context, 100)
{
	m_clock.restart();

    m_context.window = &m_window;
    m_context.eventManager = m_window.GetEventManager();
	m_context.textureManager = &m_textureManager;
	m_context.entityManager = &m_entityManager;

    m_stateManager.SwitchTo(StateType::Game);
}

Game::~Game()
{
	m_context.entityManager->Purge();
	m_context.textureManager->PurgeResources();
}

void Game::Update()
{
    m_window.Update(); // Update window events
    m_stateManager.Update(m_elapsed);
}

void Game::LateUpdate()
{
    m_stateManager.ProcessRequests();
    RestartClock();
}

void Game::Render()
{
    m_window.BeginDraw(); // Clear the window
    m_stateManager.Draw();

	// Debug.
	if (m_context.debugOverlay.Debug())
	{
		m_context.debugOverlay.Draw(m_window.GetRenderWindow());
	}
	// End debug.

    m_window.EndDraw();
}