#pragma once
#include "Map.h"

/*
   ===========================================================
   class Layer is going to hold all types of layers. at the
   moment we only have tilelayers to work with, but hopefully
   we'll be able to implement more layers soon.
   ===========================================================
 */
class Layer
{
public:
    virtual ~Layer()
    {
    }

    virtual void            Update()                                = 0;
    virtual void            Draw()                                  = 0;
    virtual Tile*           GetTile(unsigned int x, unsigned int y) = 0;
    virtual std::string     GetLayerName()                          = 0;

protected:
    std::string m_layerName;

private:
};
