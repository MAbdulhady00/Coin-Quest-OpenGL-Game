#pragma once

#include "../ecs/component.hpp"
#include <string>

namespace our
{
    // this component denotes that the object is a collidable body
    class CollisionComponent : public Component
    {

    public:
        // the radius at which the object will be detected
        // as a collided body by other objects
        double detectionRadius = 1.0;
        // enum of the sound that will be played when the object collides
        int soundName;
        // path to the sound file
        std::string soundPath;
        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads collision from the given json object
        void deserialize(const nlohmann::json &data) override;
    };
}