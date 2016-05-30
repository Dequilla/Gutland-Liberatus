#include "GameStateSplashScreen.h"
#include "StateManager.h"
#include <iostream>

GameStateSplashScreen::GameStateSplashScreen(StateManager * statemanager) : BaseState(statemanager)
{
}

GameStateSplashScreen::~GameStateSplashScreen()
{
    //delete m_spriteSheet;
    OnDestroy();
}

void GameStateSplashScreen::OnCreate()
{
	m_lambBuffer.loadFromFile("media/Sound/Effects/Lamm.ogg");
	m_lamb.setBuffer(m_lambBuffer);
	m_lamb.setVolume(100); 
	m_lamb.setLoop(false);

	m_enkelBuffer.loadFromFile("media/Sound/Effects/Enkel.ogg");
	m_enkel.setBuffer(m_enkelBuffer);
	m_enkel.setVolume(50);
	m_enkel.setLoop(false);

    m_elapsedTime    = 0.0f;
    m_opacity        = 0;
    m_opacityChecker = 0.05f;
    m_fading         = false;
    m_finished       = false;

    sf::Vector2u windowSize = m_stateMgr->GetContext()->
                              window->GetRenderWindow()->getSize();

    m_spriteSheet = new Kengine::SpriteSheet(this->GetStateManager()->GetContext()->textureManager);
    if (!m_spriteSheet->LoadSheet("media/SpriteSheets/LLISplash.sheet"))
    {
        std::cout << "Loading LLISPLASH failed" << std::endl;
    }

    m_spriteSheet->SetSpritePosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 1.5f));
    m_spriteSheet->SetAnimation("Idle", false, false);
    m_spriteSheet->GetCurrentAnimation()->Reset();

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
    m_spriteSheet->ReleaseSheet();
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
                m_opacity        += 5;

                m_sprite.setColor(sf::Color(255, 255, 255, m_opacity));
            }
            else if (m_opacity == 255 && m_elapsedTime > 2.5f)
            {
                m_fading = true;
				m_enkel.play();
            }
        }
        else if (m_fading)
        {
            if (m_opacity > 0 && m_elapsedTime >= m_opacityChecker)
            {
                m_opacityChecker += 0.05f;
                m_opacity        -= 5;

                m_sprite.setColor(sf::Color(255, 255, 255, m_opacity));
            }
            else if ((m_opacity == 0) && (m_elapsedTime > 6.0f))
            {
                //Start animation for next splash
                m_spriteSheet->SetAnimation("Idle", true, true);
                m_splashScreenIndex += 1;
                m_elapsedTime        = 0.0f;
            }
        }
    }
    //If splash screen index is 1 do Lonely Lamb Interactive splash
    else if (m_splashScreenIndex == 1)
    {
        m_spriteSheet->Update(time.asSeconds());

        if (m_secondSplashCheck && m_elapsedTime > 7.0f)
        {
            //if it has run continue for 5 sec continue
            this->m_splashScreenIndex += 1;
            std::cout << "Changing index";
        }

        //Check everytime the animations stop
        if (!m_spriteSheet->GetCurrentAnimation()->IsPlaying())
        {
            std::cout << m_elapsedTime << std::endl;

            if (!m_secondSplashCheck)
            {
                //If it hasent run yet run it
                m_spriteSheet->SetAnimation("Walk", true, false);
                m_secondSplashCheck = true;

				m_lamb.play();
            }
        }
    }
    else
    {
		
        m_finished = true;
    }

    if (m_finished)
    {
        //Continue();
    }
    m_elapsedTime += time.asSeconds();
}

void GameStateSplashScreen::Draw()
{
    sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();
    if (m_splashScreenIndex == 0)
    {
        window->draw(m_sprite);
    }
    if (m_splashScreenIndex != 0)
    {
        m_spriteSheet->Draw(window);
    }
}

void GameStateSplashScreen::Continue(Kengine::EventDetails * details)
{
    m_stateMgr->SwitchTo(StateType::MainMenu);
    m_stateMgr->Remove(StateType::SplashScreen);
}
