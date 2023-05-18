#pragma once

#include "../../ecs/tag-component.hpp"

namespace our
{
    class CoinTagComponent : public TagComponent
    {
    public:
        static std::string getID() { return "CoinTag"; }

        void deserialize(const nlohmann::json &data) override
        {
            // No data to deserialize
            return;
        }
    };
}