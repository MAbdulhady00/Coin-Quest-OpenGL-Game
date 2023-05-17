#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/coin.hpp"
#include "../components/player.hpp"
#include "../components/heart.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <random>

#define MAX_HEART 3
#define FAR_VERTICAL_DISTANCE 250
#define FAR_HORIZONTAL_DISTANCE 7

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class GemsGeneratorSystem
    {
    private:
        static int last_gen;

    public:
        void init()
        {
            GemsGeneratorSystem::last_gen = 0;
        }
        /**
         * @brief Create a Coin Mesh Component object from json file
         *
         * @param mesh MeshRendererComponent pointer
         */
        void CreateGemMeshComponent(MeshRendererComponent *mesh, std::string mesh_string = "gem_heart", std::string material_string = "gem_heart")
        {
            mesh->mesh = AssetLoader<Mesh>::get(mesh_string);
            mesh->material = AssetLoader<Material>::get(material_string);
        }

        /**
         * @brief Create a Coin Movement Component object
         *
         * @param movement MovementComponent pointer
         */
        void CreateGemMovementComponent(MovementComponent *movement)
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
                if (entity->getComponent<HeartComponent>())
                {
                    count++;

                    // If the coin passed the player, remove it
                    if (entity->localTransform.position.z > playerPosition.z + 1.0f)
                    {
                        world->markForRemoval(entity);
                        printf("Removed Gem was too far\n");
                    }
                    // If the coin is close to the player, remove it and count a point
                    if (glm::distance(playerPosition, entity->localTransform.position) < 1.0f)
                    {
                        world->markForRemoval(entity);
                        printf("Removed Collected Gem\n");
                    }
                }
            }
            // Delete all the marked entities
            // world->deleteMarkedEntities();
            // If the number of coins is less than the max number of coins, add a new coin
            while (count < MAX_HEART)
            {
                Entity *newGem = world->add();
                newGem->name = "Heart";
                // Random location for the coin
                newGem->localTransform.position = glm::vec3(
                    (float)(distr(generator) % FAR_HORIZONTAL_DISTANCE - FAR_HORIZONTAL_DISTANCE / 2.0),
                    -0.5f,
                    (float)(distr(generator) * -1) + playerPosition.z);
                // print the coin position
                printf("New Gem Position: %f, %f, %f\n", newGem->localTransform.position.x, newGem->localTransform.position.y, newGem->localTransform.position.z);
                newGem->localTransform.scale = glm::vec3(0.015f, 0.015f, 0.015f);
                newGem->localTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                // add mesh component
                MeshRendererComponent *mesh = newGem->addComponent<MeshRendererComponent>();
                CreateGemMeshComponent(mesh);
                // add movement component
                MovementComponent *movement = newGem->addComponent<MovementComponent>();
                CreateGemMovementComponent(movement);
                // add gem component
                HeartComponent *gem = newGem->addComponent<HeartComponent>();
                count++;
                last_gen = FAR_VERTICAL_DISTANCE;
            }
        }
    };
    int GemsGeneratorSystem::last_gen = 0;
}
