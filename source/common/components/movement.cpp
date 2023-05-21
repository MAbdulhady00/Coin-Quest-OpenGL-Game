#include "movement.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    // Reads linearVelocity & angularVelocity from the given json object
    void MovementComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        linearVelocity = data.value("linearVelocity", linearVelocity);
        angularVelocity = glm::radians(data.value("angularVelocity", angularVelocity));
        linearAcceleration = data.value("linearAcceleration", linearAcceleration);
        angularAcceleration = glm::radians(data.value("angularAcceleration", angularAcceleration));
        maxLinearVelocityComponent = data.value("maxLinearVelocityComponent", maxLinearVelocityComponent);
        maxAngularVelocityComponent = glm::radians(data.value("maxAngularVelocityComponent", maxAngularVelocityComponent));
        mass = data.value("mass", mass);
    }
}