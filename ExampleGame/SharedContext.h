#pragma once

#include <Kengine/Window.h>
#include <Kengine/EventManager.h>
#include <Kengine/TextureManager.h>
#include "EntityManager.h"
#include <Kengine/DebugOverlay.h>

class Map;

struct SharedContext
{
	SharedContext() :
		window(nullptr),
		eventManager(nullptr),
		textureManager(nullptr),
		entityManager(nullptr),
		gameMap(nullptr)
	{
	}

	Kengine::Window* window;
	Kengine::EventManager* eventManager;
	Kengine::TextureManager* textureManager;
	EntityManager* entityManager;
	Map* gameMap;
	Kengine::DebugOverlay debugOverlay;

};