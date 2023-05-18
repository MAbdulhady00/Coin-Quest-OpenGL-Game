#pragma once

#include "../../ecs/tag-component.hpp"

namespace our
{
    class GeneratedTagComponent : public TagComponent
    {
    public:
        static std::string getID() { return "GeneratedTag"; }
    };
}