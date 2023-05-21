#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/player.hpp"
#include "../components/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class MovementSystem
    {
        float lastDeltaTime;

    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            // long long timeDiff = time(NULL) - initialTime;
            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // Get the movement component if it exists
                MovementComponent *movement = entity->getComponent<MovementComponent>();

                // If the movement component exists
                if (movement)
                {
                    // change the linear velocity based on the linear acceleration
                    movement->linearVelocity += movement->linearAcceleration * deltaTime;
                    // change the angular velocity based on the angular acceleration
                    movement->angularVelocity += movement->angularAcceleration * deltaTime;
                    // clamp the linear velocity to the maximum linear velocity
                    movement->linearVelocity = glm::clamp(movement->linearVelocity, -movement->maxLinearVelocityComponent, movement->maxLinearVelocityComponent);
                    // clamp the angular velocity to the maximum angular velocity
                    movement->angularVelocity = glm::clamp(movement->angularVelocity, -movement->maxAngularVelocityComponent, movement->maxAngularVelocityComponent);
                    // change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;

                    // handle gravity
                    if (entity->localTransform.position.y > 0)
                        movement->linearVelocity.y -= 9.8 * movement->mass * deltaTime;
                    // entity has mass and is on the ground
                    else if (movement->mass > 0)
                    {
                        // set the linear velocity of y axis to 0
                        movement->linearVelocity.y = 0;
                        // set the position to zero this will be useful to reset the position of the local transform after 
                        // the player jumps to avoid that the position decrease than zero
                        entity->localTransform.position.y = 0;
                    }
                }
            }
            lastDeltaTime = deltaTime;
        }
    };

}