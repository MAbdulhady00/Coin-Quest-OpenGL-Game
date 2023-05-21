#include "collision.hpp"

namespace our
{
    void CollisionComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        //get the radius of the collision area
        detectionRadius = data.value("detectionRadius", detectionRadius);
        // get the sound name of the collision type
        soundName = data.value("soundName", soundName);
        // get the sound path of the collision type
        soundPath = data.value("soundPath", soundPath);
    }
}