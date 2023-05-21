#pragma once

#include "../ecs/component.hpp"

namespace our
{
    class GeneratedComponent : public Component
    {
    public:
        // will determine if the component will be removed from the world if it's behind the player
        double destructionOffset = -10.0;
        virtual void deserialize(const nlohmann::json &data) override
        {
            // nothing to deserialize
        }
        static std::string getID() { return "GeneratedTag"; }
    };
}