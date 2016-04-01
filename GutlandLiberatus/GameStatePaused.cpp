#include "GameStatePaused.h"
#include "StateManager.h"

GameStatePaused::~GameStatePaused()
{
}

void GameStatePaused::OnCreate()
{
	SetTransparent(true);

	if (!m_font.loadFromFile("fonts/liberation-mono.ttf"))
	{
		std::cout << "Could not load fonts/liberation-mono.ttf!" << std::endl;
	}
	m_text.setFont(m_font);
	m_text.setString(sf::String("PAUSED"));
	m_text.setCharacterSize(14);
	m_text.setStyle(sf::Text::Bold);

	sf::Vector2u windowSize = m_stateMgr->GetContext()->window->GetRenderWindow()->getSize();

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
					 textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setPosition(0.0f, 0.0f);
	m_rect.setFillColor(sf::Color(0, 0, 0, 150));

	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::Paused, "Key_P",
					   &GameStatePaused::Unpause, this);
}

void GameStatePaused::OnDestroy()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::Paused, "Key_P");
}

void GameStatePaused::Activate()
{
}

void GameStatePaused::Deactivate()
{
}

void GameStatePaused::Update(const sf::Time & time)
{
}

void GameStatePaused::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();
	window->draw(m_rect);
	window->draw(m_text);
}

void GameStatePaused::Unpause(Kengine::EventDetails * details)
{
	m_stateMgr->SwitchTo(StateType::Game);
}
