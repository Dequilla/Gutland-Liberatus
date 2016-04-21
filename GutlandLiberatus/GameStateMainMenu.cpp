#include "GameStateMainMenu.h"
#include "StateManager.h"

GameStateMainMenu::GameStateMainMenu(StateManager* stateManager) : Kengine::BaseState(stateManager)
{

}

GameStateMainMenu::~GameStateMainMenu()
{
	OnDestroy();
}

void GameStateMainMenu::OnCreate()
{
	if (!m_textureBackground.loadFromFile("media/Textures/background_steampunk.png"))
	{
		std::cout << "Could not load background!" << std::endl;
	}
	m_spriteBackground.setTexture(m_textureBackground);

	if (!m_font.loadFromFile("media/Fonts/Steamwreck.otf"))
	{
		std::cout << "Could not load fonts/Steamwreck.otf" << std::endl;
	}
	m_text.setFont(m_font);
	m_text.setString(sf::String("GUTLAND LIBERATUS"));
	m_text.setCharacterSize(72);
	m_text.setColor(sf::Color(0, 0, 0, 255));

	sf::Vector2u windowSize = m_stateMgr->GetContext()->
		window->GetRenderWindow()->getSize();
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
					 textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, 100.0f);

	m_buttonSize = sf::Vector2f(200.0f, 50.0f);
	m_buttonPos = sf::Vector2f(windowSize.x / 2.4f, 200.0f);
	m_buttonPadding = 4;

	std::string str[5];
	str[0] = "PLAY";
	str[1] = "CREDITS";
	str[2] = "OPTIONS";
	str[3] = "EXIT";
	str[4] = "COMBAT TEST";

	for (int i = 0; i < 5; ++i)
	{
		sf::Vector2f buttonPosition(m_buttonPos.x, m_buttonPos.y + (i * (m_buttonSize.y + m_buttonPadding)));	
		m_buttons.addButton(str[i], buttonPosition, m_buttonSize, sf::Color(0, 0, 0, 255), sf::Color(148, 58, 13, 255), sf::Color(173, 89, 47, 255));
	}

	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->AddCallback(StateType::MainMenu, "Mouse_Left",
					   &GameStateMainMenu::MouseClick, this);
	evMgr->AddCallback(StateType::MainMenu, "Key_Up",
					   &GameStateMainMenu::KeyUp, this);
	evMgr->AddCallback(StateType::MainMenu, "Key_Down",
					   &GameStateMainMenu::KeyDown, this);
	evMgr->AddCallback(StateType::MainMenu, "Key_Enter",
					   &GameStateMainMenu::KeyEnter, this);
}

void GameStateMainMenu::OnDestroy()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
	evMgr->RemoveCallback(StateType::MainMenu, "Key_Up");
	evMgr->RemoveCallback(StateType::MainMenu, "Key_Down");
	evMgr->RemoveCallback(StateType::MainMenu, "Key_Enter");
}

void GameStateMainMenu::Activate()
{
	if (m_stateMgr->HasState(StateType::Game) &&
		m_buttons.getButton("PLAY")->getString() == std::string("PLAY"))
	{
		m_buttons.getButton("PLAY")->setString(std::string("RESUME"));
	}
}

void GameStateMainMenu::Deactivate()
{
}

void GameStateMainMenu::Update(const sf::Time& time)
{
	//NOTE TO SELF add mouse_pos relative to window to context
	m_buttons.update(m_stateMgr->GetContext()->eventManager->GetMousePos(m_stateMgr->GetContext()->window->GetRenderWindow()), m_stateMgr->GetContext()->window->GetRenderWindow());
}

void GameStateMainMenu::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();
	
	window->draw(m_spriteBackground);
	window->draw(m_text);

	m_buttons.draw();
}

void GameStateMainMenu::MouseClick(Kengine::EventDetails * details)
{
	sf::Vector2i mousePos = details->m_mouse;
	checkButtons();
}

void GameStateMainMenu::KeyUp(Kengine::EventDetails* details)
{
	
}

void GameStateMainMenu::KeyDown(Kengine::EventDetails* details)
{
	
}

void GameStateMainMenu::KeyEnter(Kengine::EventDetails* details)
{
	
}

void GameStateMainMenu::checkButtons()
{
	std::string active = m_buttons.getActiveButton();
	if (active == "PLAY")
	{
		m_stateMgr->SwitchTo(StateType::Game);
	}
	else if (active == "CREDITS")
	{
		std::cout << "RICHARD CREDITS PLOX" << std::endl;
	}
	else if (active == "OPTIONS")
	{
		m_stateMgr->SwitchTo(StateType::Options);
	}
	else if (active == "EXIT")
	{
		m_stateMgr->GetContext()->window->Close();
	}
	else if (active == "COMBAT TEST")
	{
		m_stateMgr->SwitchTo(StateType::Combat);
	}
}
