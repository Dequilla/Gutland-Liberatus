#pragma once

#include <vector>
#include <unordered_map>
#include "GameStateSplashScreen.h"
#include "GameStateIntro.h"
#include "GameStateMainMenu.h"
#include "GameStateGame.h"
#include "GameStatePaused.h"
#include "GameStateOptionsMenu.h"
#include "GameStateCombat.h"
#include "SharedContext.h"


enum class StateType
{
    Intro = 1, MainMenu, Game, Paused, GameOver, Credits, SplashScreen, Options, Combat
};

using StateContainer = std::vector < std::pair < StateType, Kengine::BaseState* >>;
using TypeContainer  = std::vector < StateType >;

// map a specific state type to a specific function
// that will generate that type
using StateFactory = std::unordered_map < StateType, std::function < Kengine::BaseState*(void) >>;

class StateManager
{
public:
    StateManager(SharedContext * shared);
    ~StateManager();

    void Update(const sf::Time& time);
    void Draw();

    void ProcessRequests();

    SharedContext* GetContext()
    {
        return m_shared;
    }
    bool HasState(const StateType& type);

    void SwitchTo(const StateType& type);
    void Remove(const StateType& type);

private:
    // Members
    SharedContext  *m_shared;
    StateContainer m_states;
    TypeContainer  m_toRemove;
    StateFactory   m_stateFactory;

    // Functions
    void CreateState(const StateType& type);
    void RemoveState(const StateType& type);

    template < class T >
    void RegisterState(const StateType& type)
    {
        // maps type in the m_stateFactory map
        // to a function that returns a pointer to newly allocated memory
        // We use templates in order to reduce the amount of code
        m_stateFactory[type] = [this]()->Kengine::BaseState *
        {
            return new T(this);
        };
    }
};
