#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/coin.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#define MAX_COIN 1

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class CoinGeneratorSystem
    {
    public:
        /**
         * @brief Create a Coin Mesh Component object from json file
         *
         * @param mesh MeshRendererComponent pointer
         */
        void CreateCoinMeshComponent(MeshRendererComponent *mesh)
        {
            mesh->mesh = AssetLoader<Mesh>::get("coin");
            mesh->material = AssetLoader<Material>::get("coin");
        }

        /**
         * @brief Create a Coin Movement Component object
         *
         * @param movement MovementComponent pointer
         */
        void CreateCoinMovementComponent(MovementComponent *movement)
        {
            movement->linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
            movement->angularVelocity = glm::radians(glm::vec3(0.0f, 100.0f, 0.0f));
        }

        // This should be called every frame to update all entities containing a CoinComponent.
        void update(World *world, float deltaTime)
        {
            // Count the number of coins in the world
            int count = 0;
            glm::vec3 playerPosition = glm::vec3(0, 0, 0);
            // For each entity in the world get the player position
            for (auto entity : world->getEntities())
            {
                // Get the camera and controller components to get the player position
                CameraComponent *camera = entity->getComponent<CameraComponent>();
                FreeCameraControllerComponent *controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    playerPosition = camera->getOwner()->localTransform.position;
            }

            // For each entity in the world get the coins
            for (auto entity : world->getEntities())
            {

                // check if the entity has a CoinComponent
                if (entity->getComponent<CoinComponent>())
                {
                    count++;

                    // If the coin passed the player, remove it
                    if (entity->localTransform.position.z > playerPosition.z + 1.0f)
                    {
                        world->markForRemoval(entity);
                        printf("Removed Coin was too far\n");
                    }
                    // If the coin is close to the player, remove it and count a point
                    if (glm::distance(playerPosition, entity->localTransform.position) < 0.5f)
                    {
                        world->markForRemoval(entity);
                        printf("Removed Collected Coin\n");
                    }
                }
            }
            // Delete all the marked entities
            world->deleteMarkedEntities();

            // If the number of coins is less than the max number of coins, add a new coin
            while (count < MAX_COIN)
            {
                Entity *newCoin = world->add();
                newCoin->name = "Coin";
                newCoin->localTransform.position = glm::vec3(0.0f, -1.0f, 0.0f);
                newCoin->localTransform.scale = glm::vec3(0.1f, 0.1f, 0.1f);
                newCoin->localTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                // add mesh component
                MeshRendererComponent *mesh = newCoin->addComponent<MeshRendererComponent>();
                CreateCoinMeshComponent(mesh);
                // add movement component
                MovementComponent *movement = newCoin->addComponent<MovementComponent>();
                CreateCoinMovementComponent(movement);
                // add coin component
                CoinComponent *coin = newCoin->addComponent<CoinComponent>();
                count++;
            }
        }
    };

}
