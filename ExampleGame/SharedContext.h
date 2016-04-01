#pragma once

#include <Kengine/Window.h>
#include <Kengine/EventManager.h>
#include "TextureManager.h"
#include "EntityManager.h"
#include "DebugOverlay.h"

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
	TextureManager* textureManager;
	EntityManager* entityManager;
	Map* gameMap;
	DebugOverlay debugOverlay;

};