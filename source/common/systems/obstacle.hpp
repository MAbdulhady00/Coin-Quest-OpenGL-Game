#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/obstacle.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include "../components/player.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#define MAX_obstacle 25
#define FAR_VERTICAL_DISTANCE 50
#define FAR_HORIZONTAL_DISTANCE 10

namespace our
{

    class ObstacleSystem
    {
    public:
        /**
         * @brief Create an obstacle Mesh Component object from json file
         *
         * @param mesh MeshRendererComponent pointer
         */
        void CreateObstacleMeshComponent(MeshRendererComponent *mesh)
        {
            mesh->mesh = AssetLoader<Mesh>::get("obstacle");
            mesh->material = AssetLoader<Material>::get("obstacle");
        }

        // /**
        //  * @brief Create a obstacle Movement Component object
        //  *
        //  * @param movement MovementComponent pointer
        //  */
        // void CreateobstacleMovementComponent(MovementComponent *movement)
        // {
        //     movement->linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
        //     movement->angularVelocity = glm::radians(glm::vec3(0.0f, 100.0f, 0.0f));
        // }

        // This should be called every frame to update all entities containing an ObstacleComponent.
        void update(World *world, float deltaTime)
        {
            // set seed for random number generator to be the current time
            srand(time(NULL));
            // Count the number of obstacles in the world
            int count = 0;
            glm::vec3 playerPosition = glm::vec3(0, 0, 0);
            // For each entity in the world get the player position
            for (auto entity : world->getEntities())
            {
                // Get the camera and controller components to get the player position
                PlayerComponent *player = entity->getComponent<PlayerComponent>();
                if (player)
                {
                    playerPosition = player->getOwner()->localTransform.position;
                    break;
                }
            }

            // For each entity in the world get the obstacles
            for (auto entity : world->getEntities())
            {
                // check if the entity has an obstacleComponent
                if (entity->getComponent<ObstacleComponent>())
                {
                    count++;

                    // If the obstacle passed the player, remove it
                    if (entity->localTransform.position.z > playerPosition.z + 1.0f)
                    {
                        world->markForRemoval(entity);
                        printf("Removed obstacle was too far\n");
                    }
                    // If the obstacle is close to the player, remove it and count a point
                    if (glm::distance(playerPosition, entity->localTransform.position) < 1.0f)
                    {
                        // show game over screen
                        printf("Game Over\n");
                    }
                }
            }
            // Delete all the marked entities
            world->deleteMarkedEntities();
            // If the number of obstacles is less than the max number of obstacles, add a new obstacle
            while (count < MAX_obstacle)
            {
                Entity *newObstacle = world->add();
                newObstacle->name = "obstacle";
                // Random location for the obstacle
                newObstacle->localTransform.position = glm::vec3(
                    (float)(rand() % FAR_HORIZONTAL_DISTANCE - FAR_HORIZONTAL_DISTANCE / 2.0),
                    0.0f,
                    (float)(rand() % FAR_VERTICAL_DISTANCE * -1) + playerPosition.z);
                // print the obstacle position
                printf("New obstacle Position: %f, %f, %f\n", newObstacle->localTransform.position.x, newObstacle->localTransform.position.y, newObstacle->localTransform.position.z);
                newObstacle->localTransform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
                newObstacle->localTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                // add mesh component
                MeshRendererComponent *mesh = newObstacle->addComponent<MeshRendererComponent>();
                CreateObstacleMeshComponent(mesh);
                // add movement component
                MovementComponent *movement = newObstacle->addComponent<MovementComponent>();
                // CreateObstacleMovementComponent(movement);
                // add obstacle component
                ObstacleComponent *obstacle = newObstacle->addComponent<ObstacleComponent>();
                count++;
            }
        }
    };

}
