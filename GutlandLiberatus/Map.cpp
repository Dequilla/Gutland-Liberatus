#include "Map.h"
#include "StateManager.h"

Map::Map(SharedContext* context, Kengine::BaseState* currentState) :
	m_context(context), m_defaultTile(context), m_maxMapSize(32, 32),
	m_tileCount(0), m_tileSetCount(0), m_mapGravity(512.0f),
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
	return Sheet::Tile_Size;
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
			ParseTileLayer(e);
		}
	}
	std::cout << "--- Map Loaded! ---" << std::endl;
}

void Map::LoadNext()
{
	m_loadNextMap = true;
}

void Map::Update(float dt)
{
	if (m_loadNextMap)
	{
		PurgeMap();
		m_loadNextMap = false;
		if (m_nextMap != "")
		{
			LoadMap("media/Maps/" + m_nextMap);
		}
		else
		{
			m_currentState->GetStateManager()->SwitchTo(StateType::GameOver);
		}
		m_nextMap = "";
	}
	sf::FloatRect viewSpace = m_context->window->GetViewSpace();
	m_background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::Draw()
{
	sf::RenderWindow* window = m_context->window->GetRenderWindow();
	window->draw(m_background);

	// These 3 lines of code makes sure that anything
	// that is not currently within the viewspace of
	// will not be drawn. Culling.
	sf::FloatRect viewSpace = m_context->window->GetViewSpace();
	sf::Vector2i tileBegin(floor(viewSpace.left / Sheet::Tile_Size), 
						   floor(viewSpace.top / Sheet::Tile_Size));
	sf::Vector2i tileEnd(ceil((viewSpace.left + viewSpace.width) / Sheet::Tile_Size),
						 ceil((viewSpace.top + viewSpace.height) / Sheet::Tile_Size));

	unsigned int count = 0;
	for (int x = tileBegin.x; x <= tileEnd.x; ++x)
	{
		for (int y = tileBegin.y; y <= tileEnd.y; ++y)
		{
			if (x < 0 || y < 0) { continue; }
			Tile* tile = GetTile(x, y);
			if (!tile) { continue; }
			sf::Sprite& sprite = tile->properties->m_sprite;
			sprite.setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
			window->draw(sprite);
			++count;

			// Debug.
			if (m_context->debugOverlay.Debug())
			{
				if (tile->properties->m_deadly || tile->warp)
				{
					sf::RectangleShape* tileMarker = new sf::RectangleShape(
						sf::Vector2f(Sheet::Tile_Size, Sheet::Tile_Size));
					tileMarker->setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
					if (tile->properties->m_deadly)
					{
						tileMarker->setFillColor(sf::Color(255, 0, 0, 100));
					}
					else if (tile->warp)
					{
						tileMarker->setFillColor(sf::Color(0, 255, 0, 150));
					}
					m_context->debugOverlay.Add(tileMarker);
				}
			}
			// End debug.
		}
	}
}

unsigned int Map::ConvertCoords(unsigned int x, unsigned int y)
{
	return (x * m_maxMapSize.x) + y; // Row-major
}

void Map::LoadTiles(const std::string& path)
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		std::cout << "Failed loading tileset file: " << path << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|') { continue; }
		std::stringstream keystream(line);
		int tileId;
		keystream >> tileId;
		if (tileId < 0) { continue; }
		TileInfo* tile = new TileInfo(m_context);
		tile->CreateTile("TileSheet", tileId);
		keystream >> tile->name >> tile->m_friction.x >> tile->m_friction.y >> tile->m_deadly;

		if (!m_tileSet.emplace(tileId, tile).second)
		{
			// Duplicate tile detected!
			std::cout << "Duplicate tile type: " << tile->name << std::endl;
			delete tile;
		}
	}
	file.close();
}

void Map::LoadTiles(TiXmlElement* tilesetRoot)
{
	std::cout << "Loading tiles" << std::endl;

	TileInfo tile = TileInfo(m_context);

	tilesetRoot->Attribute("tilecount", &m_tileCount);

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
}

void Map::ParseTileLayer(TiXmlElement* tileElement)
{
	// TODO(Richard) - Extend this function in order for
	// us to parse multiple tile layers

	// Search for the node we need
	for (TiXmlElement* e = tileElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("data"))
		{
			ParseTilePositions(e);

		}
	}
}

void Map::ParseTilePositions(TiXmlElement * tileData)
{
	int x = 0, y = 0;

	for (TiXmlElement* e = tileData->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (x >= m_mapWidth)
		{
			x = 0;
			++y;
		}
		if (e->Value() == std::string("tile"))
		{

			int tileID = 0;
			e->Attribute("gid", &tileID);

			tileID--;

			if (tileID < 0) { ++x; continue; }

			auto itr = m_tileSet.find(tileID);

			Tile* tile = new Tile();

			tile->properties = itr->second;

			if (!m_tileMap.emplace(ConvertCoords(x, y), tile).second)
			{
				// Duplicate tile detected!
				std::cout << "! Duplicate tile! : " << x
					<< "" << y << std::endl;
				delete tile;
				tile = nullptr;
				continue;
			}
			++x;
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
	m_tileMap.clear();
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
