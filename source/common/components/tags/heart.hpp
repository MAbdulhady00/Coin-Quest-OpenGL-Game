#pragma once

#include "../../ecs/tag-component.hpp"

namespace our
{
    class HeartTagComponent : public TagComponent
    {
    public:
        static std::string getID() { return "HeartTag"; }

        void deserialize(const nlohmann::json &data) override
        {
            // No data to deserialize
            return;
        }
    };
}