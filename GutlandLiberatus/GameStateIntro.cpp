#include "GameStateIntro.h"

// We use forward declaration of the StateManager class/struct
// in BaseState.h and therefor we need to include StateManager.h
// in the implementation file of all our states.
#include "StateManager.h" 

GameStateIntro::~GameStateIntro()
{
	OnDestroy();
}

void GameStateIntro::OnCreate()
{
    m_timePassed = 0.0f;

    sf::Vector2u windowSize = m_stateMgr->GetContext()->
        window->GetRenderWindow()->getSize();

    if (!m_introTexture.loadFromFile("media/Textures/intro.png"))
    {
        std::cout << "Could not open intro texture!" << std::endl;
    }
    
    m_introSprite.setTexture(m_introTexture);
    m_introSprite.setOrigin(m_introTexture.getSize().x / 2.0f,
        m_introTexture.getSize().y);
    m_introSprite.setPosition(windowSize.x / 2.0f, 0.0f);
    
    if (!m_font.loadFromFile("media/Fonts/liberation-mono.ttf"))
    {
        std::cout << "Could not load font liberation-mono.ttf" << std::endl;
    }
    m_text.setFont(m_font);
    m_text.setString({ "Press SPACE to continue" });
    m_text.setCharacterSize(15);
    
    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    
    Kengine::EventManager* evMgr = m_stateMgr->
        GetContext()->eventManager;
        
	evMgr->AddCallback(StateType::Intro, "Intro_Continue",
					   &GameStateIntro::Continue, this);
}

void GameStateIntro::OnDestroy()
{
    Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;
    evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
}

void GameStateIntro::Activate()
{
}

void GameStateIntro::Deactivate()
{
}

void GameStateIntro::Update(const sf::Time &time)
{
    if (m_timePassed < 5.0f) // Less than five seconds
    {
        m_timePassed += time.asSeconds();
        m_introSprite.setPosition(m_introSprite.getPosition().x,
            m_introSprite.getPosition().y + (64 * time.asSeconds()));
    }
}

void GameStateIntro::Draw()
{
    sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();
    window->draw(m_introSprite);
    
    if (m_timePassed >= 5.0f)
    {
        window->draw(m_text);
    }
}

void GameStateIntro::Continue(Kengine::EventDetails* details)
{
    if (m_timePassed >= 5.0f)
    {
        m_stateMgr->SwitchTo(StateType::MainMenu);
        m_stateMgr->Remove(StateType::Intro);
    }
}


