#include "GameStateOptionsMenu.h"
#include "StateManager.h"

GameStateOptionsMenu::GameStateOptionsMenu(StateManager* stateManager) : Kengine::BaseState(stateManager)
{
	m_options = m_stateMgr->GetContext()->optionsManager;
}

GameStateOptionsMenu::~GameStateOptionsMenu()
{
	//delete m_options;
	OnDestroy();
}

void GameStateOptionsMenu::OnCreate()
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

	m_fullscreenText.setFont(m_font);
	if(m_stateMgr->GetContext()->window->IsFullscreen() == false) m_fullscreenText.setString(sf::String("OFF"));
	else if (m_stateMgr->GetContext()->window->IsFullscreen() == true) m_fullscreenText.setString(sf::String("ON"));
	m_fullscreenText.setPosition(780, 200);
	m_fullscreenText.setCharacterSize(40);
	m_fullscreenText.setColor(sf::Color(0, 0, 0, 255));

	m_verticalsyncText.setFont(m_font);
	if (m_stateMgr->GetContext()->window->GetVerticalSync() == false) m_verticalsyncText.setString(sf::String("OFF"));
	else if (m_stateMgr->GetContext()->window->GetVerticalSync() == true) m_verticalsyncText.setString(sf::String("ON"));
	m_verticalsyncText.setPosition(780, 260);
	m_verticalsyncText.setCharacterSize(40);
	m_verticalsyncText.setColor(sf::Color(0, 0, 0, 255));

		

	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->AddCallback(StateType::Options, "Key_Escape",
		&GameStateOptionsMenu::MainMenu, this);
	evMgr->AddCallback(StateType::Options, "Key_O",
		&GameStateOptionsMenu::ToggleOverlay, this);
	evMgr->AddCallback(StateType::Options, "Mouse_Left",
		&GameStateOptionsMenu::MouseClick, this);

	std::string str[3];
	str[0] = "TOGGLE FULLSCREEN";
	str[1] = "TOGGLE VERTICALSYNC";
	str[2] = "BACK";
	for (int i = 0; i < 3; i++)
	{					
		m_buttons.addButton(str[i], sf::Vector2f(450, 200 + (i * 60)), sf::Vector2f(300, 50), sf::Color(0, 0, 0, 255), sf::Color(148, 58, 13, 255), sf::Color(173, 89, 47, 255));
	}
}

void GameStateOptionsMenu::OnDestroy()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->RemoveCallback(StateType::Options, "Key_Escape");
	evMgr->RemoveCallback(StateType::Options, "Key_O");
}

void GameStateOptionsMenu::Activate()
{

}

void GameStateOptionsMenu::Deactivate()
{

}

void GameStateOptionsMenu::Update(const sf::Time& time)
{
	m_buttons.update(m_stateMgr->GetContext()->eventManager->GetMousePos(m_stateMgr->GetContext()->window->GetRenderWindow()), m_stateMgr->GetContext()->window->GetRenderWindow());
}

void GameStateOptionsMenu::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();

	window->draw(m_spriteBackground);
	window->draw(m_text);
	window->draw(m_fullscreenText);
	window->draw(m_verticalsyncText);
	m_buttons.draw();
}

void GameStateOptionsMenu::MainMenu(Kengine::EventDetails* details)
{
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void GameStateOptionsMenu::ToggleOverlay(Kengine::EventDetails* details)
{
	std::cout << "ToggleOverlay" << std::endl;
	m_stateMgr->GetContext()->debugOverlay.SetDebug(!m_stateMgr->GetContext()->debugOverlay.Debug());
}

void GameStateOptionsMenu::checkButtons(Kengine::EventDetails* details)
{
	bool temp_verticalsync = m_stateMgr->GetContext()->window->GetVerticalSync();
	

	std::string active = m_buttons.getActiveButton();
	if (active == "TOGGLE FULLSCREEN")
	{
		m_stateMgr->GetContext()->window->ToggleFullscreen(details);
		bool temp = m_stateMgr->GetContext()->window->IsFullscreen();
		if (temp == false)
		{
			m_options->setStringOptionAt("fullscreen", "false");
			m_fullscreenText.setString(sf::String("OFF"));
		}
		else if (temp == true)
		{
			m_options->setStringOptionAt("fullscreen", "true");
			m_fullscreenText.setString(sf::String("ON"));
		}
		
	}
	else if (active == "TOGGLE VERTICALSYNC")
	{
		std::cout << "TOGGLING VERTICALSYNC" << std::endl;
		m_stateMgr->GetContext()->window->SetVerticalSync(!temp_verticalsync);
		
		if (temp_verticalsync == true)
		{
			m_options->setStringOptionAt("verticalsync", "false");
		}
		else if (temp_verticalsync == false)
		{
			m_options->setStringOptionAt("verticalsync", "true");
		}
	}
	else if (active == "BACK")
	{
		m_stateMgr->SwitchTo(StateType::MainMenu);
	}

	m_options->writeOptionsToFile();

	//Set identifier correctly after each buttonpress
	if (m_stateMgr->GetContext()->window->GetVerticalSync() == false)
	{
		m_verticalsyncText.setString(sf::String("OFF"));
	}
	else if (m_stateMgr->GetContext()->window->GetVerticalSync() == true)
	{
		m_verticalsyncText.setString(sf::String("ON"));
	}
}

void GameStateOptionsMenu::MouseClick(Kengine::EventDetails* details)
{
	checkButtons(details);
}