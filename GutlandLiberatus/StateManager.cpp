#include "StateManager.h"

StateManager::StateManager(SharedContext* shared) :
m_shared(shared)
{
	RegisterState<GameStateSplashScreen>(StateType::SplashScreen);
    RegisterState<GameStateIntro>(StateType::Intro);
    RegisterState<GameStateMainMenu>(StateType::MainMenu);
	RegisterState<GameStateGame>(StateType::Game);
    RegisterState<GameStatePaused>(StateType::Paused);
	RegisterState<GameStateOptionsMenu>(StateType::Options);
	RegisterState<GameStateCombat>(StateType::Combat);
}

StateManager::~StateManager()
{
    for (auto &itr : m_states)
    {
        itr.second->OnDestroy();
        delete itr.second;
    }
}
void StateManager::Draw()
{
    if (m_states.empty()){ return; }
    if (m_states.back().second->IsTransparent()
            && m_states.size() > 1)
    {
        auto itr = m_states.end();
        while (itr != m_states.begin())
        {
            if (itr != m_states.end())
            {
                if (!itr->second->IsTransparent())
                {
                    break;
                }
            }
            --itr;
        }
        for (; itr != m_states.end(); ++itr)
        {
			// Set the window view to the state view before it gets rendered
			m_shared->window->GetRenderWindow()->setView(itr->second->GetView());
            itr->second->Draw();
        }
    } 
    else 
    {
        m_states.back().second->Draw();
    }
}

void StateManager::Update(const sf::Time& time)
{
    if (m_states.empty()){ return; }
    if (m_states.back().second->IsTranscendent()
            && m_states.size() > 1)
    {
        auto itr = m_states.end();
        while (itr != m_states.begin())
        {
            if (itr != m_states.end()){
                if (!itr->second->IsTranscendent())
                {
                    break;
                }
            }
            --itr;
        }
        for (; itr != m_states.end(); ++itr)
        {
            itr->second->Update(time);
        }
    } 
    else 
    {
        m_states.back().second->Update(time);
    }
}

bool StateManager::HasState(const StateType& type)
{
    for (auto itr = m_states.begin();
        itr != m_states.end(); ++itr)
    {
        if (itr->first == type)
        {
            auto removed = std::find(m_toRemove.begin(),
                m_toRemove.end(), type);

            if (removed == m_toRemove.end()) { return true; }
            return false;
        }
    }
    return false;
}

void StateManager::Remove(const StateType& type)
{
    m_toRemove.push_back(type);
}

void StateManager::ProcessRequests()
{
    while (m_toRemove.begin() != m_toRemove.end())
    {
        RemoveState(*m_toRemove.begin());
        m_toRemove.erase(m_toRemove.begin());
    }
}

void StateManager::SwitchTo(const StateType& type)
{
    m_shared->eventManager->SetCurrentState(type);
    for (auto itr = m_states.begin();
        itr != m_states.end(); ++itr)
    {
        if (itr->first == type)
        {
            m_states.back().second->Deactivate();
            StateType tmp_type = itr->first;
            Kengine::BaseState* tmp_state = itr->second;
            m_states.erase(itr);
            m_states.emplace_back(tmp_type, tmp_state);
            tmp_state->Activate();

			m_shared->window->GetRenderWindow()->setView(tmp_state->GetView());
            return;
        }
    }
    // State with type wasn't found.
    if (!m_states.empty()) { m_states.back().second->Deactivate(); }
    CreateState(type);
    m_states.back().second->Activate();

	m_shared->window->GetRenderWindow()->setView(m_states.back().second->GetView());
}

void StateManager::CreateState(const StateType& type)
{
    auto newState = m_stateFactory.find(type);

    if (newState == m_stateFactory.end()) { return; }

    Kengine::BaseState* state = newState->second();

	// Get the default view for our current state
	state->m_view = m_shared->window->GetRenderWindow()->getDefaultView();
    
	m_states.emplace_back(type, state);
    state->OnCreate();
}

void StateManager::RemoveState(const StateType& type)
{
    for (auto itr = m_states.begin();
        itr != m_states.end(); ++itr)
    {
        if (itr->first == type)
        {
            itr->second->OnDestroy();
            delete itr->second;
            m_states.erase(itr);
            return;
        }
    }
}
