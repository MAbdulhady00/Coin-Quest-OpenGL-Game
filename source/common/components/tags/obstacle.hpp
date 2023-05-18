#pragma once

#include "../../ecs/tag-component.hpp"

namespace our
{
    class ObstacleTagComponent : public TagComponent
    {
    public:
        static std::string getID() { return "ObstacleTag"; }
    };
}