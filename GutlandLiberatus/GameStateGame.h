#pragma once

#include <Kengine/EventManager.h>
#include <Kengine/BaseState.h>

#include "Map.h"

class GameStateGame: public Kengine::BaseState
{
public:
    GameStateGame(StateManager * stateManager);
    ~GameStateGame();

    void OnCreate();
    void OnDestroy();

    void Activate();
    void Deactivate();

    void Update(const sf::Time& time);
    void Draw();

    void MainMenu(Kengine::EventDetails* details);
    void Pause(Kengine::EventDetails* details);
    void ToggleOverlay(Kengine::EventDetails* details);

private:
    Map* m_gameMap;
};
