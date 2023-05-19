#include "collision.hpp"

namespace our
{
    void CollisionComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        detectionRadius = data.value("detectionRadius", detectionRadius);
        soundName = data.value("soundName", soundName);
        soundPath = data.value("soundPath", soundPath);
        postProcess = data.value("postProcess", postProcess);
        postProcessIndex = data.value("postProcessIndex", postProcessIndex);
    }
}