#pragma once

#include "../../ecs/tag-component.hpp"

namespace our
{
    class WarnTagComponent : public TagComponent
    {
    public:
        static std::string getID() { return "WarnTag"; }
    };
}