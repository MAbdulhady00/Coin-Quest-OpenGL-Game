#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    // This component denotes that the MovementSystem will move the owning entity by a certain linear and angular velocity.
    // This component is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/movement.hpp"
    // For a more complex example of how to use the ECS framework, see "free-camera-controller.hpp"
    class MovementComponent : public Component
    {
    public:
        glm::vec3 linearVelocity = {0, 0, 0};      // Each frame, the entity should move as follows: position += linearVelocity * deltaTime
        glm::vec3 angularVelocity = {0, 0, 0};     // Each frame, the entity should rotate as follows: rotation += angularVelocity * deltaTime
        glm::vec3 linearAcceleration = {0, 0, 0};  // Each frame, the linearVelocity should change as follows: linearVelocity += linearAcceleration * deltaTime
        glm::vec3 angularAcceleration = {0, 0, 0}; // Each frame, the angularVelocity should change as follows: angularVelocity += angularAcceleration * deltaTime

        float maxLinearVelocityComponent = 1000.0f;  // The maximum linear velocity that the entity can reach
        float maxAngularVelocityComponent = 1000.0f; // The maximum angular velocity that the entity can reach

        float mass = 0.0f; // The mass of the entity. Having no mass means that the entity will not be affected by forces.

        // The ID of this component type is "Movement"
        static std::string getID() { return "Movement"; }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json &data) override;
    };

}