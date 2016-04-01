#include "GameStateSplashScreen.h"
#include "StateManager.h"
#include <iostream>

GameStateSplashScreen::~GameStateSplashScreen()
{
	//delete m_spriteSheet;
}

void GameStateSplashScreen::OnCreate()
{
	m_elapsedTime = 0.0f;
	m_opacity = 0;
	m_opacityChecker = 0.05f;
	m_fading = false;
	m_finished = false;

	m_spriteSheet = new Kengine::SpriteSheet(this->GetStateManager()->GetContext()->textureManager);
	if (!m_spriteSheet->LoadSheet("media/SpriteSheets/LLISplash.sheet"))
	{
		std::cout << "Loading LLISPLASH failed" << std::endl;
	}

	sf::Vector2u windowSize = m_stateMgr->GetContext()->
		window->GetRenderWindow()->getSize();

	if (!m_texture.loadFromFile("media/Textures/splashscreen.png"))
	{
		std::cout << "Could not open splash screen texture!" << std::endl;
	}
	m_sprite.setTexture(m_texture);
	m_sprite.setColor(sf::Color(255, 255, 255, m_opacity));
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f,
					   m_texture.getSize().y / 2.0f);
	m_sprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->AddCallback(StateType::SplashScreen, "Intro_Continue",
					   &GameStateSplashScreen::Continue, this);
}

void GameStateSplashScreen::OnDestroy()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::SplashScreen, "Intro_Continue");
}

void GameStateSplashScreen::Activate()
{
}

void GameStateSplashScreen::Deactivate()
{
}

void GameStateSplashScreen::Update(const sf::Time & time)
{
	if (m_splashScreenIndex == 0)
	{
		if (!m_fading)
		{
			if (m_opacity < 255 && m_elapsedTime >= m_opacityChecker)
			{
				m_opacityChecker += 0.05f;
				m_opacity += 5;

				m_sprite.setColor(sf::Color(255, 255, 255, m_opacity));
			}
			else if (m_opacity == 255 && m_elapsedTime > 2.5f)
			{
				m_fading = true;
			}
		}
		else if (m_fading)
		{
			if (m_opacity > 0 && m_elapsedTime >= m_opacityChecker)
			{
				m_opacityChecker += 0.05f;
				m_opacity -= 5;

				m_sprite.setColor(sf::Color(255, 255, 255, m_opacity));
			}
			else if ((m_opacity == 0) && (m_elapsedTime > 6.0f))
			{
				m_splashScreenIndex += 1;
				m_spriteSheet->SetAnimation("Idle", true, true);
				m_spriteSheet->SetSpritePosition(sf::Vector2f(400.0f, 640.0f));
			}
		}
	}
	//If splash screen index is 1 do Lonely Lamb Interactive splash
	else if (m_splashScreenIndex == 1)
	{
		//m_splashScreenIndex += 1;
		//Todo add code for LLI splash
		m_spriteSheet->Update(time.asSeconds());
		
	}
	else
	{
		m_finished = true;
	}

	if (m_finished)
	{
		m_stateMgr->SwitchTo(StateType::Intro);
		m_stateMgr->Remove(StateType::SplashScreen);
	}
	m_elapsedTime += time.asSeconds();
}

void GameStateSplashScreen::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();
	window->draw(m_sprite);
	m_spriteSheet->Draw(window);
}

void GameStateSplashScreen::Continue(Kengine::EventDetails * details)
{
	m_stateMgr->SwitchTo(StateType::Intro);
	m_stateMgr->Remove(StateType::SplashScreen);
}
