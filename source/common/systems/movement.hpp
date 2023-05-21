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
        long long initialTime;
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
                // Get the player component if it exists
                PlayerComponent *player = entity->getComponent<PlayerComponent>();
                // Get the camera component if it exists
                CameraComponent *camera = entity->getComponent<CameraComponent>();

                // If the movement component exists
                if (movement)
                {
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    // If the player component exists and the player is not on the ground 
                    if (entity->localTransform.position.y > 0 && player)
                        // add the effect of gravity
                        movement->linearVelocity.y -= 9.8 * (deltaTime * 1.5);
                    // if the player component exists check if the player position of y axis is less than or equal to 0
                    if (entity->localTransform.position.y <= 0 && player)
                    {
                        // set the linear velocity of y axis to 0
                        movement->linearVelocity.y = 0;
                        // set the position to zero this will be useful to reset the position of the local transform after 
                        // the player jumps to avoid that the position decrease than zero
                        entity->localTransform.position.y = 0;
                    }
                    // update the rotation speed to add the effect of angular velocity
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                    // continously update the velocity of the entity till reach to the maximum velocity (80.0)
                    if ((player || camera) && movement->linearVelocity.z >= -80.0)
                    {
                        movement->linearVelocity[2] -= deltaTime * 0.25;
                        if (player)
                            movement->angularVelocity[0] -= deltaTime * 0.0625;
                    }
                }
            }
            lastDeltaTime = deltaTime;
        }
    };

}