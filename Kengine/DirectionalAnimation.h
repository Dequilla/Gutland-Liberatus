#pragma once
#include "BaseAnimation.h"
#include "Direction.h"

namespace Kengine
{
    class DirectionalAnimation: public BaseAnimation
    {
protected:
        void FrameStep();
        void CropSprite();
        void ReadIn(std::stringstream& stream);
    };
}
