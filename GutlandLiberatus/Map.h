#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <map>
#include <array>
#include <fstream>
#include <sstream>
#include <Kengine/BaseState.h>
#include "SharedContext.h"
#include "tinyxml.h"

class Layer;

enum Sheet { Tile_Size = 32, Sheet_Width = 256, Sheet_Height = 256 };

using TileID = unsigned int;

/*
   ===========================================================
   struct TileInfo holds information about specific tiles such
   as friction, position, texture, or other custom properties
   ===========================================================
 */
struct TileInfo
{
    TileInfo(SharedContext * context, const std::string& texture = "", TileID id = 0) :
        m_context(context), m_id(0), m_deadly(false), m_friction(sf::Vector2f(0.8f, 0.8f))
    {
    }

    ~TileInfo()
    {
        if (m_texture == "")
        {
            return;
        }
        m_context->textureManager->ReleaseResource(m_texture);
    }

    void CreateTile(const std::string& texture = "", TileID id = 0)
    {
        Kengine::TextureManager* tMgr = m_context->textureManager;
        if (texture == "")
        {
            m_id = id; return;
        }
        if (!tMgr->RequireResource(texture))
        {
            return;
        }

        m_texture = texture;
        m_id      = id;

        m_sprite.setTexture(*tMgr->GetResource(m_texture));

        // For some reason the map gets buggy if we use margin when compiling for release
/*		sf::IntRect tileBoundaries(margin + (m_id % (sheetWidth / tileWidth) * (tileWidth + spacing)),
                                                                   margin + (m_id / (sheetWidth / tileHeight) * (tileHeight + spacing)),
                                                                   tileWidth, tileHeight);*/
        sf::IntRect tileBoundaries(m_id % (sheetWidth / tileWidth) * tileWidth,
                                   m_id / (sheetWidth / tileHeight) * tileHeight,
                                   tileWidth, tileHeight);

        m_sprite.setTextureRect(tileBoundaries);
    }

    sf::Sprite    m_sprite;

    TileID        m_id;
    std::string   name;
    std::string   warpName;
    std::string   warpValue;
    sf::Vector2f  m_friction;
    bool          m_deadly;

    SharedContext *m_context;
    std::string   m_texture;

    int           tileSize;
    int           mapWidth;
    int           mapHeight;
    int           sheetWidth;
    int           sheetHeight;
    int           firstGridID;
    int           tileWidth;
    int           tileHeight;
    int           spacing;
    int           margin;
    int           numColumns;
};

/*
   ===========================================================
   ===========================================================
 */
struct Tile
{
    TileInfo* properties;
    bool    warp;             // Is the tile a warp.
    // Other flags unique to each tile.
};

using TileMap = std::unordered_map < TileID, Tile* >;
using TileSet = std::unordered_map < TileID, TileInfo* >;

/*
   ===========================================================
   class Map handles loading of maps.
   ===========================================================
 */
class Map
{
public:
    Map(SharedContext * context, Kengine::BaseState * currentState);
    ~Map();

    Tile*                   GetTile(unsigned int x, unsigned int y);
    TileInfo*               GetDefaultTile();
    float                   GetGravity() const;
    std::vector < Layer * > *GetLayers()
    {
        return &m_layers;
    }
    unsigned int            GetTileSize() const;
    const sf::Vector2u&     GetMapSize() const;
    const sf::Vector2f&     GetPlayerStart() const;

    void                    LoadMap(const std::string& path);
    void                    LoadNext(const std::string& nextMap);

    void                    Update(float dt);
    void                    Draw();

    // Method for converting 2D coordinates to 1D ints.
    unsigned int            ConvertCoords(const unsigned int &x, const unsigned int &y);

private:
    TileSet            m_tileSet;
    TileMap            m_tileMap;
    TileInfo           m_defaultTile;
    sf::Sprite         m_background;
    sf::Vector2u       m_maxMapSize;
    sf::Vector2f       m_playerStart;
    std::string        m_nextMap;
    std::string        m_backgroundTexture;
    std::string        m_layerName;
    bool               m_loadNextMap;
    SharedContext      *m_context;
    int                m_tileCount;
    int                m_tileSize;
    int                m_mapWidth;
    int                m_mapHeight;
    int                m_playerId = -1;
    float              m_mapGravity;
    unsigned int       m_tileSetCount;
    Kengine::BaseState *m_currentState;

    std::vector < Layer * > m_layers;

    void               LoadTiles(const std::string& path);
    void               LoadTiles(TiXmlElement* tilesetRoot);
    void               ParseTileLayer(TiXmlElement* tileElement);
    void               ParseObjectLayer(TiXmlElement* objectElement);
    void               PurgeMap();
    void               PurgeTileSet();
};
