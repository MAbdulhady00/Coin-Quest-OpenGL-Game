#pragma once

#include "../../ecs/tag-component.hpp"

namespace our
{
    class CoinTagComponent : public TagComponent
    {
    public:
        static std::string getID() { return "CoinTag"; }
    };
}