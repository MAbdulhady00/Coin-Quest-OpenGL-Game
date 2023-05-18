#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include "../components/player.hpp"
#include "../components/tags/obstacle.hpp"
#include "../components/collision.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#define MAX_obstacle 15
#define FAR_VERTICAL_DISTANCE 250
#define FAR_HORIZONTAL_DISTANCE 10

namespace our
{

    class ObstacleSystem
    {

    private:
        static int last_gen;

    public:
        void init()
        {
            ObstacleSystem::last_gen = 0;
        }
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

        // This should be called every frame to update all entities containing an ObstacleComponent.
        void update(World *world, float deltaTime)
        {
            std::random_device rand_dev;
            std::mt19937 generator(rand_dev());
            std::uniform_int_distribution<int> distr(last_gen, last_gen + FAR_VERTICAL_DISTANCE);

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
                if (entity->getComponent<ObstacleTagComponent>())
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
                        world->markForRemoval(entity);
                        printf("Removed obstacle collided obstacle\n");
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
                    (float)(distr(generator) % FAR_HORIZONTAL_DISTANCE - FAR_HORIZONTAL_DISTANCE / 2.0),
                    -0.5f,
                    (float)(distr(generator) * -1) + playerPosition.z);
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
                ObstacleTagComponent *obstacle = newObstacle->addComponent<ObstacleTagComponent>(); // add collision component
                CollisionComponent *collision = newObstacle->addComponent<CollisionComponent>();
                count++;
            }
        }
    };
    int ObstacleSystem::last_gen = 0;

}
