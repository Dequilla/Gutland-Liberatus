#pragma once

#include <Kengine/Window.h>
#include <Kengine/EventManager.h>
#include <Kengine/TextureManager.h>
#include <Kengine/DebugOverlay.h>
#include "EntityManager.h"
#include <Kengine/Kopter.h>

class Map;

struct SharedContext
{
    SharedContext() :
        window(nullptr),
        eventManager(nullptr),
        textureManager(nullptr),
        entityManager(nullptr),
        gameMap(nullptr),
        optionsManager(nullptr)
    {
    }

    Kengine::Window         *window;
    Kengine::EventManager   *eventManager;
    Kengine::TextureManager *textureManager;
    Kengine::DebugOverlay   debugOverlay;
    EntityManager           *entityManager;
    Map                     *gameMap;
    Kengine::Kopter         *optionsManager;
};
