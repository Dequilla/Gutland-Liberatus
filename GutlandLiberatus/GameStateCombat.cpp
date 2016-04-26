#include "GameStateCombat.h"
#include "StateManager.h"

GameStateCombat::GameStateCombat(StateManager* stateManager) : Kengine::BaseState(stateManager)
{}

GameStateCombat::~GameStateCombat()
{
	OnDestroy();
}

void GameStateCombat::OnCreate()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->AddCallback(StateType::Combat, "Mouse_Left",
		&GameStateCombat::MouseClick, this);
	evMgr->AddCallback(StateType::Combat, "Key_Escape",
		&GameStateCombat::MainMenu, this);
	evMgr->AddCallback(StateType::Combat, "Key_P",
		&GameStateCombat::Pause, this);
	evMgr->AddCallback(StateType::Combat, "Key_O",
		&GameStateCombat::ToggleOverlay, this);

	sf::Vector2u size = m_stateMgr->GetContext()->window->GetWindowSize();
	
	//Selection and creation of fighting background
	//For now a static background
	m_backgroundTexture.loadFromFile("media/Textures/TempBackground.png");
	m_background.setTexture(m_backgroundTexture);
	m_background.setPosition(sf::Vector2f(0.f, 0.f));

	//Portrait is static for now
	m_portraitTexture.loadFromFile("media/Textures/placeholderport.png");
	m_portrait.setTexture(m_portraitTexture);
	m_portrait.setPosition(sf::Vector2f(520.f, 540.f));
	m_portrait.setScale(sf::Vector2f(5.1f, 5.1f));   
	
	//Portraitframe is static dont move
	m_portraitFrameTexture.loadFromFile("media/Textures/portraitFrame.png");
	m_portraitFrame.setTexture(m_portraitFrameTexture);
	m_portraitFrame.setPosition(sf::Vector2f(510.f, 540.f));
	m_portraitFrame.setScale(sf::Vector2f(4.6f, 4.6f));

	//The bottom frame is static dont move
	m_bottomFrameTexture.loadFromFile("media/Textures/BottomFrame.png");
	m_bottomFrame.setTexture(m_bottomFrameTexture);
	m_bottomFrame.setPosition(sf::Vector2f(0.f, 520.f));
	m_bottomFrame.setScale(sf::Vector2f(18.3f, 3.0f));

	//Health text and font
	m_steamwreckFont.loadFromFile("media/Fonts/Steamwreck.otf");
	m_healthText.setFont(m_steamwreckFont);
	m_healthText.setPosition(sf::Vector2f(50.f, 560.f));
	m_healthText.setString(sf::String("HEALTH: 1337"));
	m_healthText.setCharacterSize(38);
	m_healthText.setColor(sf::Color(20, 20, 20, 255));

	//Hitchance text
	m_meleeHitChanceText.setFont(m_steamwreckFont);
	m_rangeHitChanceText.setFont(m_steamwreckFont);
	m_meleeHitChanceText.setPosition(sf::Vector2f(50.f, 590.f));
	m_rangeHitChanceText.setPosition(sf::Vector2f(50.f, 620.f));
	m_meleeHitChanceText.setString(sf::String("ME. HIT: 0%"));
	m_rangeHitChanceText.setString(sf::String("RA. HIT: 100%"));
	m_meleeHitChanceText.setCharacterSize(38);
	m_rangeHitChanceText.setCharacterSize(38);
	m_meleeHitChanceText.setColor(sf::Color(20, 20, 20, 255));
	m_rangeHitChanceText.setColor(sf::Color(20, 20, 20, 255));

	//Evasion text
	m_evasionText.setFont(m_steamwreckFont);
	m_evasionText.setPosition(sf::Vector2f(50.f, 650.f));
	m_evasionText.setCharacterSize(38);
	m_evasionText.setString("EVASION: 50%");
	m_evasionText.setColor(sf::Color(20, 20, 20, 255));
	
	std::string buttonString[3];
	buttonString[0] = "FIGHT";
	buttonString[1] = "ACTION";
	buttonString[2] = "ESCAPE";
	for (int i = 0; i < 3; i++)
	{
		m_buttons.addButton(buttonString[i], sf::Vector2f(700.f, 542.f + (58.f * i)), sf::Vector2f(200.f, 55.f), sf::Color(20, 20, 20, 255), sf::Color(50, 50, 50, 255), sf::Color(70, 70, 70, 255));
	}
}

void GameStateCombat::OnDestroy()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_P");
	evMgr->RemoveCallback(StateType::Game, "Key_O");
}

void GameStateCombat::Activate()
{
}

void GameStateCombat::Deactivate()
{
}

void GameStateCombat::Update(const sf::Time& time)
{
	SharedContext* context = m_stateMgr->GetContext();
	//TODO Create a class to handle textobjects with addText() just like button
	//ERRORINCUDE ++ dick #mature

	m_healthText.setString("HEALTH:       " + sf::String(int2Str(m_characterContext.getCurrentHealth())));
	m_meleeHitChanceText.setString(sf::String("M.HITCHANCE:  " + int2Str((m_characterContext.getMeleeHitChance() + m_characterContext.getRangeHitChanceModifier()))));
	m_rangeHitChanceText.setString(sf::String("R.HITCHANCE:  " + int2Str((m_characterContext.getRangeHitChance() + m_characterContext.getRangeHitChanceModifier()))));
	m_evasionText.setString(sf::String("EVASION:      " + int2Str((m_characterContext.getEvasion() + m_characterContext.getEvasionModifier()))));

	sf::Vector2i mousePos = m_stateMgr->GetContext()->eventManager->GetMousePos(m_stateMgr->GetContext()->window->GetRenderWindow());
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();

	m_enemyController.update(mousePos, window);
	m_buttons.update(mousePos, window);
	//m_frame.Update(time.asSeconds());
	m_stateMgr->GetContext()->entityManager->Update(time.asSeconds());
}

void GameStateCombat::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();

	window->draw(m_background);

	m_enemyController.draw(window);

	window->draw(m_bottomFrame);

	m_buttons.draw();
	window->draw(m_portraitFrame);
	window->draw(m_portrait);
	window->draw(m_healthText);
	window->draw(m_meleeHitChanceText);
	window->draw(m_rangeHitChanceText);
	window->draw(m_evasionText);

	
	
}

void GameStateCombat::MainMenu(Kengine::EventDetails* details)
{
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void GameStateCombat::Pause(Kengine::EventDetails* details)
{
	m_stateMgr->SwitchTo(StateType::Paused);
}

void GameStateCombat::ToggleOverlay(Kengine::EventDetails* details)
{
	std::cout << "ToggleOverlay" << std::endl;
	m_stateMgr->GetContext()->debugOverlay.SetDebug(!m_stateMgr->GetContext()->debugOverlay.Debug());
}

void GameStateCombat::MouseClick(Kengine::EventDetails * details)
{
	//Button logic
	std::string active = m_buttons.getActiveButton();
	if (active == "FIGHT")
	{
		std::cout << "FIGHT" << std::endl;
	}else if (active == "ACTION")
	{
		std::cout << "ACTION" << std::endl;
		m_enemyController.generateNewRandomEnemies();
	}
	else if (active == "ESCAPE")
	{
		std::cout << "ESCAPE" << std::endl;
		m_enemyController.resetCombat();
	}

	//Enemy logic
	m_enemyController.changeSelected();

}
