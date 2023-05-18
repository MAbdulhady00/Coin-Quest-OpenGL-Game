#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/tags/coin.hpp"
#include "../components/collision.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include "../components/player.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <random>
#include <time.h>

#define MAX_COIN 100
#define FAR_VERTICAL_DISTANCE 250
#define FAR_HORIZONTAL_DISTANCE 7

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class CoinGeneratorSystem
    {
    private:
        static int last_gen;

    public:
        void init()
        {
            CoinGeneratorSystem::last_gen = 0;
        }
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
            std::random_device rand_dev;
            std::mt19937 generator(rand_dev());
            std::uniform_int_distribution<int> distr(last_gen, last_gen + FAR_VERTICAL_DISTANCE);

            // Count the number of coins in the world
            int count = 0;
            glm::vec3 playerPosition = glm::vec3(0, 0, 0);
            // For each entity in the world get the player position
            for (auto entity : world->getEntities())
            {
                // Get the camera and controller components to get the player position
                PlayerComponent *player = entity->getComponent<PlayerComponent>();
                if (player)
                    playerPosition = player->getOwner()->localTransform.position;
            }

            // For each entity in the world get the coins
            for (auto entity : world->getEntities())
            {

                // check if the entity has a CoinComponent
                if (entity->getComponent<CoinTagComponent>())
                {
                    count++;

                    // If the coin passed the player, remove it
                    if (entity->localTransform.position.z > playerPosition.z + 1.0f)
                    {
                        world->markForRemoval(entity);
                        printf("Removed Coin was too far\n");
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
                // Random location for the coin
                newCoin->localTransform.position = glm::vec3(
                    (float)(distr(generator) % FAR_HORIZONTAL_DISTANCE - FAR_HORIZONTAL_DISTANCE / 2.0),
                    -0.75f,
                    (float)(distr(generator) * -1) + playerPosition.z);
                // print the coin position
                printf("New Coin Position: %f, %f, %f\n", newCoin->localTransform.position.x, newCoin->localTransform.position.y, newCoin->localTransform.position.z);
                newCoin->localTransform.scale = glm::vec3(0.1f, 0.1f, 0.1f);
                newCoin->localTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                // add mesh component
                MeshRendererComponent *mesh = newCoin->addComponent<MeshRendererComponent>();
                CreateCoinMeshComponent(mesh);
                // add movement component
                MovementComponent *movement = newCoin->addComponent<MovementComponent>();
                CreateCoinMovementComponent(movement);
                // add collision component
                CollisionComponent *collision = newCoin->addComponent<CollisionComponent>();
                // add coin component
                CoinTagComponent *coin = newCoin->addComponent<CoinTagComponent>();
                count++;
                last_gen = FAR_VERTICAL_DISTANCE;
            }
        }
    };
    int CoinGeneratorSystem::last_gen = 0;
}
