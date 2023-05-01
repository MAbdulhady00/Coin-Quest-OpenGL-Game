#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
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
        void CreateCoinMeshComponent(MeshRendererComponent *mesh)
        {
            mesh->mesh = AssetLoader<Mesh>::get("coin");
            mesh->material = AssetLoader<Material>::get("coin");
        }

        void CreateCoinMovementComponent(MovementComponent *movement)
        {
            movement->linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
            movement->angularVelocity = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        // This should be called every frame to update all entities containing a CoinComponent.
        void update(World *world, float deltaTime)
        {
            // For each entity in the world
            int count = 0;
            for (auto entity : world->getEntities())
            {
                // If the coin component exists
                if (entity->name == "Coin")
                {
                    count++;
                }
            }
            while (count <= MAX_COIN)
            {
                Entity *newCoin = world->add();
                newCoin->name = "Coin";
                newCoin->localTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
                newCoin->localTransform.scale = glm::vec3(0.1f, 0.1f, 0.1f);
                newCoin->localTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                MeshRendererComponent *mesh = newCoin->addComponent<MeshRendererComponent>();
                CreateCoinMeshComponent(mesh);
                MovementComponent *movement = newCoin->addComponent<MovementComponent>();
                CreateCoinMovementComponent(movement);
                count++;
            }
        }
    };

}
