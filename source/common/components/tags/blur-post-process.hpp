#pragma once

#include "../../ecs/tag-component.hpp"

namespace our
{
    class BlurTagComponent : public TagComponent
    {
    public:
        static std::string getID() { return "BlurTag"; }
    };
}