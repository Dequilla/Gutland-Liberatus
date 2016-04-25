#pragma once
#include "Map.h"

class Layer
{
public:
    virtual ~Layer()
    {
    }

    virtual void Update() = 0;
    virtual void Draw()   = 0;

    virtual Tile* GetTile(unsigned int x, unsigned int y) = 0;
    virtual std::string GetLayerName()                    = 0;

protected:

    std::string m_layerName;

private:
};
