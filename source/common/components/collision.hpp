#pragma once

#include "../ecs/component.hpp"
#include <string>

namespace our
{
    // this component denos that the object is a collision body
    class CollisionComponent : public Component
    {

    public:
        // the radius at which the object will be detected
        // as a collided body by other objects
        double detectionRadius = 1.0;
        // the name of the sound collision
        int soundName;
        // the sound path that will be generated when the collision happens
        std::string soundPath;
        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads collision from the given json object
        void deserialize(const nlohmann::json &data) override;
    };
}