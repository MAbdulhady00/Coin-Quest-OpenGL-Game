#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/coin.hpp"
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
            movement->angularVelocity = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        // This should be called every frame to update all entities containing a CoinComponent.
        void update(World *world, float deltaTime)
        {
            // Count the number of coins in the world
            int count = 0;
            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // check if the entity has a CoinComponent
                if (entity->getComponent<CoinComponent>())
                {
                    count++;
                }
            }
            // If the number of coins is less than the max number of coins, add a new coin
            while (count < MAX_COIN)
            {
                Entity *newCoin = world->add();
                newCoin->name = "Coin";
                newCoin->localTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
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
