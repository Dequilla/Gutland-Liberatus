#include "Map.h"
#include "StateManager.h"
#include "TileLayer.h"

Map::Map(SharedContext* context, Kengine::BaseState* currentState) :
	m_context(context), m_defaultTile(context), m_maxMapSize(32, 32),
	m_tileCount(0), m_tileSetCount(0), m_mapGravity(0.0f),
	m_loadNextMap(false), m_currentState(currentState)
{
	m_context->gameMap = this;
//	LoadTiles("media/Tiles.cfg");
}

Map::~Map()
{
	PurgeMap();
	PurgeTileSet();
	m_context->gameMap = nullptr;
}

Tile* Map::GetTile(unsigned int x, unsigned int y)
{	
	// Needs to search in the layer tilemap
	auto itr = m_tileMap.find(ConvertCoords(x, y));
	return(itr != m_tileMap.end() ? itr->second : nullptr);
}

TileInfo* Map::GetDefaultTile()
{
	return &m_defaultTile;
}

float Map::GetGravity() const
{
	return m_mapGravity;
}

unsigned int Map::GetTileSize() const
{
	return m_tileSize;
}

const sf::Vector2u& Map::GetMapSize() const
{
	return m_maxMapSize;
}

const sf::Vector2f& Map::GetPlayerStart() const
{
	return m_playerStart;
}

void Map::LoadMap(const std::string& path)
{
	std::cout << "--- Loading a map: " << path << std::endl;

	TiXmlDocument mapDocument;
	mapDocument.LoadFile(path);

	TiXmlElement* root = mapDocument.RootElement();

	root->Attribute("tilewidth", &m_tileSize);
	root->Attribute("width", &m_mapWidth);
	root->Attribute("height", &m_mapHeight);

	m_maxMapSize.x = m_mapWidth;
	m_maxMapSize.y = m_mapHeight;

	for (TiXmlElement* e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("tileset"))
		{
			LoadTiles(e);
		}
	}

	for (TiXmlElement* e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("layer"))
		{
			m_layerName = e->Attribute("name");
			ParseTileLayer(e);
		}
	}

	for (TiXmlElement* e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("objectgroup"))
		{
			ParseObjectLayer(e);
		}
	}

	std::cout << "--- Map Loaded! ---" << std::endl;

	root->Clear();
	root = nullptr;
}

void Map::LoadNext(const std::string& nextMap)
{
	m_nextMap = nextMap;
	m_loadNextMap = true;
}

void Map::Update(float dt)
{
	if (m_loadNextMap)
	{
		PurgeMap();
		PurgeTileSet();
		m_loadNextMap = false;
		if (m_nextMap != "")
		{
			LoadMap("media/Maps/" + m_nextMap + ".tmx");
		}
		else
		{
			m_currentState->GetStateManager()->SwitchTo(StateType::GameOver);
		}
		m_nextMap = "";
	}
	sf::FloatRect viewSpace = m_context->window->GetViewSpace();
	m_background.setPosition(viewSpace.left, viewSpace.top);

	for (int i = 0; i < m_layers.size(); ++i)
	{
		m_layers[i]->Update();
	}
}

void Map::Draw()
{
	for (int i = 0; i < m_layers.size(); ++i)
	{
		if (m_layers[i]->GetLayerName() != "collision")
		{
			m_layers[i]->Draw();
		}

		if (i == 1)
		{
			m_context->entityManager->Draw();
		}
	}
}

unsigned int Map::ConvertCoords(const unsigned int &x, const unsigned int &y)
{
	return (x * m_maxMapSize.x) + y; // Row-major
}

void Map::LoadTiles(TiXmlElement* tilesetRoot)
{
	std::cout << "Loading tiles" << std::endl;

	TileInfo tile = TileInfo(m_context);

	tilesetRoot->Attribute("tilecount", &m_tileCount);

	std::cout << m_tileCount << std::endl;

	for (int i = 0; i < m_tileCount; i++)
	{
		TileInfo* tileinfo = new TileInfo(m_context);

		tilesetRoot->Attribute("firstgid", &tileinfo->firstGridID);
		tilesetRoot->Attribute("tilewidth", &tileinfo->tileWidth);
		tilesetRoot->Attribute("tileheight", &tileinfo->tileHeight);
		tilesetRoot->Attribute("spacing", &tileinfo->spacing);
		tilesetRoot->Attribute("margin", &tileinfo->margin);
		tilesetRoot->Attribute("columns", &tileinfo->numColumns);
		tilesetRoot->FirstChildElement()->Attribute("width", &tileinfo->sheetWidth);
		tilesetRoot->FirstChildElement()->Attribute("height", &tileinfo->sheetHeight);
		tileinfo->name = tilesetRoot->Attribute("name");

		if (tileinfo->margin < 0) { tileinfo->margin = 0; }
		if (tileinfo->spacing < 0) { tileinfo->spacing = 0; }

		tileinfo->CreateTile(tileinfo->name, i);

		if (!m_tileSet.emplace(i, tileinfo).second)
		{
			std::cout << "Duplicate tile type: " << i << std::endl;
		}
	}
	int id = 0;
	for (TiXmlElement* e = tilesetRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("tile"))
		{
			e->Attribute("id", &id);

			auto itr = m_tileSet.find(id);

			for (TiXmlElement* p = e->FirstChildElement(); p != NULL; p = p->NextSiblingElement())
			{
				if (p->Value() == std::string("properties"))
				{
					itr->second->warpName = p->FirstChildElement()->Attribute("name");
					itr->second->warpValue = p->FirstChildElement()->Attribute("value");
				}
			}
		}
	}
}

void Map::ParseTileLayer(TiXmlElement* tileElement)
{
	// TODO(Richard) - Extend this function in order for
	// us to parse multiple tile layers

	TileLayer* layer = new TileLayer(m_context, &m_tileSet, m_layerName);

	// Search for the node we need
	for (TiXmlElement* e = tileElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("data"))
		{
			layer->CreateLayer(e, m_mapWidth);
		}
	}
	m_layers.push_back(layer);
}

void Map::ParseObjectLayer(TiXmlElement* objectElement)
{
	EntityManager* entityMgr = m_context->entityManager;

	for (TiXmlElement* e = objectElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("object"))
		{
			if (e->Attribute("type") == std::string("Player"))
			{
				// Set up the player position here.
				m_playerId = entityMgr->Add(EntityType::Player);
				if (m_playerId < 0) { continue; }
				int playerX = 0; int playerY = 0;
				int playerWidth = 0, playerHeight = 0;
				e->Attribute("x", &playerX);
				e->Attribute("y", &playerY);
				e->Attribute("width", &playerWidth);
				e->Attribute("height", &playerHeight);
				entityMgr->Find(m_playerId)->SetPosition(playerX + (playerWidth / 2), playerY + playerHeight);
				m_playerStart = sf::Vector2f(playerX + (playerWidth / 2.0f), playerY + playerHeight);
			}
			else if (e->Attribute("type") == std::string("Enemy"))
			{
				std::string enemyName;
				enemyName = e->Attribute("name");
				int enemyId = entityMgr->Add(EntityType::Enemy, enemyName);
				if (enemyId < 0) { continue; }
				int enemyX = 0; int enemyY = 0;
				e->Attribute("x", &enemyX);
				e->Attribute("y", &enemyY);
				entityMgr->Find(enemyId)->SetPosition(enemyX, enemyY);
			}
			else if (e->Attribute("type") == std::string("warp"))
			{
				std::string mapName = e->Attribute("name");
				int x = 0, y = 0, width = 0, height = 0;
				
				e->Attribute("x", &x);
				e->Attribute("y", &y);
				e->Attribute("width", &width);
				e->Attribute("height", &height);

				m_warp.warp = true;
				m_warp.mapName = mapName;
				m_warp.size = sf::Vector2f(width, height);
				m_warp.position = sf::Vector2f(x, y);
			}
		}
	}
}

void Map::PurgeMap()
{
	m_tileCount = 0;
	for (auto &itr : m_tileMap)
	{
		delete itr.second;
	}

	for (int j = 0, i = m_layers.size(); j < i; j++)
	{
		Layer* pLayers = m_layers.at(j);
		delete pLayers;
	}

	m_tileMap.clear();
	m_layers.clear();

	m_context->entityManager->Purge();

	if (m_backgroundTexture == "") { return; }
	m_context->textureManager->ReleaseResource(m_backgroundTexture);
	m_backgroundTexture = "";
}

void Map::PurgeTileSet()
{
	for (auto &itr : m_tileSet)
	{
		delete itr.second;
	}
	m_tileSet.clear();
	m_tileSetCount = 0;
}
