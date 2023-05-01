#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class CoinComponent : public Component
    {
    public:
        // The ID of this component type is "Coin"
        static std::string getID() { return "Coin"; }

        // Reads  angularVelocity from the given json object
        void deserialize(const nlohmann::json &data) override;
    };

}