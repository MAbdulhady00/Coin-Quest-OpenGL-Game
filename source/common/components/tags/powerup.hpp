#pragma once

#include "../../ecs/tag-component.hpp"

namespace our
{
    class PowerupTagComponent : public TagComponent
    {
    public:
        static std::string getID() { return "powerupTag"; }
    };
}