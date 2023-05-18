#pragma once

#include "../../ecs/tag-component.hpp"

namespace our
{
    class ObstacleTagComponent : public TagComponent
    {
    public:
        static std::string getID() { return "ObstacleTag"; }

        void deserialize(const nlohmann::json &data) override
        {
            // No data to deserialize
            return;
        }
    };
}