#include "GameStateMainMenu.h"
#include "StateManager.h"

GameStateMainMenu::~GameStateMainMenu()
{
}

void GameStateMainMenu::OnCreate()
{
	if (!m_textureBackground.loadFromFile("media/Textures/background.png"))
	{
		std::cout << "Could not load background!" << std::endl;
	}
	m_spriteBackground.setTexture(m_textureBackground);

	if (!m_font.loadFromFile("media/Fonts/AYearWithoutRain.ttf"))
	{
		std::cout << "Could not load fonts/AYearWithoutRain.ttf!" << std::endl;
	}
	m_text.setFont(m_font);
	m_text.setString(sf::String("MAIN MENU:"));
	m_text.setCharacterSize(44);

	sf::Vector2u windowSize = m_stateMgr->GetContext()->
		window->GetRenderWindow()->getSize();
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
					 textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 4.0f, 100.0f);

	m_buttonSize = sf::Vector2f(200.0f, 50.0f);
	m_buttonPos = sf::Vector2f(windowSize.x / 4.0f, 200.0f);
	m_buttonPadding = 4;

	std::string str[3];
	str[0] = "PLAY";
	str[1] = "CREDITS";
	str[2] = "EXIT";

	for (int i = 0; i < 3; ++i)
	{
		sf::Vector2f buttonPosition(m_buttonPos.x, m_buttonPos.y + (i * (m_buttonSize.y + m_buttonPadding)));
		m_buttons[i].setFont(m_font);
		m_buttons[i].setOrigin(m_buttonSize.x / 2.0f, m_buttonSize.y / 2.0f);
		m_buttons[i].setSize(m_buttonSize);
		m_buttons[i].setPosition(buttonPosition);
		m_buttons[i].setTextSize(14);
		m_buttons[i].setString(sf::String(str[i]));
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
		m_buttons[0].getString() == "PLAY")
	{
		m_buttons[0].setString(sf::String("RESUME"));
		sf::FloatRect rect = m_buttons[0].getLocalBounds();
		m_buttons[0].setOrigin(rect.left + rect.width / 2.0f,
							  rect.top + rect.height / 2.0f);
	}
}

void GameStateMainMenu::Deactivate()
{
}

void GameStateMainMenu::Update(const sf::Time& time)
{
	for (int i = 0; i < 3; ++i)
	{
		if (i == m_currentChoice)
		{
			m_buttons[i].setBackgroundColor(sf::Color(125, 125, 125, 255));
			m_buttons[i].setTextColor(sf::Color(0, 51, 102, 255));
		}
		else
		{
			m_buttons[i].setBackgroundColor(sf::Color(0, 0, 0, 255));
			m_buttons[i].setTextColor(sf::Color(255, 255, 255, 255));
		}
	}
}

void GameStateMainMenu::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();
	
	window->draw(m_spriteBackground);
	window->draw(m_text);

	for (int i = 0; i < 3; ++i)
	{
		window->draw(m_buttons[i]);
	}
}

void GameStateMainMenu::MouseClick(Kengine::EventDetails * details)
{
	sf::Vector2i mousePos = details->m_mouse;

	float halfX = m_buttonSize.x / 2.0f;
	float halfY = m_buttonSize.y / 2.0f;

	for (int i = 0; i < 3; ++i)
	{
		if (mousePos.x >= m_rects[i].getPosition().x - halfX &&
			mousePos.x <= m_rects[i].getPosition().x + halfX &&
			mousePos.y >= m_rects[i].getPosition().y - halfY &&
			mousePos.y <= m_rects[i].getPosition().y + halfY)
		{
			if (i == 0)
			{
				m_stateMgr->SwitchTo(StateType::Game);
			}
			else if (i == 1)
			{
				// Credits State
			}
			else if (i == 2)
			{
				m_stateMgr->GetContext()->window->Close();
			}
		}
	}
}

void GameStateMainMenu::KeyUp(Kengine::EventDetails* details)
{
	m_currentChoice--;
	if (m_currentChoice == -1)
	{
		m_currentChoice = 2;
	}
}

void GameStateMainMenu::KeyDown(Kengine::EventDetails* details)
{
	m_currentChoice++;
	if (m_currentChoice == 3)
	{
		m_currentChoice = 0;
	}
}

void GameStateMainMenu::KeyEnter(Kengine::EventDetails* details)
{
	if (m_currentChoice == 0)
	{
		m_stateMgr->SwitchTo(StateType::Game);
	}
	else if (m_currentChoice == 1)
	{
		// TODO(Richard) Credits State
	}
	else if (m_currentChoice == 2)
	{
		m_stateMgr->GetContext()->window->Close();
	}
}
