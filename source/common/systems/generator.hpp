#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/tags/coin.hpp"
#include "../components/collision.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include "../components/player.hpp"
#include "../components/generated.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <random>
#include <iostream>

namespace our
{

    class GeneratorSystem
    {
    private:
        double lastGeneration = 0.0;
        int generationPlaneOffset = 100;         // The distance from the player at which generated entitites are generated
        int destructionPlaneOffset = -10;        // The distance from the player at which generated entities are destroyed
        int horizontalDistance = 7;              // The horizontal distance from the player at which generated entities are generated
        double generationChance = 0.25;          // The chance that an entity will be generated
        double coinChance = 0.25;                // The chance that a coin will be generated
        double obstacleChance = 0.25;            // The chance that an obstacle will be generated
        double heartChance = 0.25;               // The chance that a heart will be generated
        double powerupChance = 0.25;             // The chance that a powerup will be generated
        double generationStep = 1.0;             // The distance between each generation attempt
        double groundLength = 100.0;             // The length of the ground
        double lastGroundGeneration = 0.0;       // The last ground generated
        double groundDestructionOffset = -100.0; // The distance from the player at which ground is destroyed
        std::mt19937 rng;
        std::uniform_real_distribution<double> distribution;
        nlohmann::json coinConfig;
        nlohmann::json obstacleConfig;
        nlohmann::json heartConfig;
        nlohmann::json powerupConfig;
        nlohmann::json groundConfig;
        glm::vec3 currentCoinRotation;

        inline Entity *randomEntityFactory(World *world, Entity *entity)
        {
            // Generate a random number between 0 and 1
            double random = distribution(rng);

            double currentChance = coinChance;
            // If the random number is less than the coin chance, generate a coin
            if (random < currentChance)
            {
                Entity *coin = world->deserializeEntity(coinConfig);
                coin->localTransform.rotation = currentCoinRotation;
                return coin;
            }
            // If the random number is less than the obstacle chance, generate an obstacle
            else if (random < currentChance + obstacleChance)
            {
                return world->deserializeEntity(obstacleConfig);
            }
            // If the random number is less than the powerup chance, generate a powerup
            else if (random < currentChance + obstacleChance + powerupChance)
            {
                return world->deserializeEntity(powerupConfig);
            }
            // must generate a heart
            else
            {
                return world->deserializeEntity(heartConfig);
            }
        }

    public:
        GeneratorSystem()
        {
            std::random_device rand_dev;
            rng = std::mt19937(rand_dev());
            distribution = std::uniform_real_distribution<double>(0.0, 1.0);
        }

        void initialize(const nlohmann::json &config)
        {
            lastGeneration = config.value("initialGeneration", lastGeneration);
            lastGroundGeneration = config.value("initialGeneration", lastGroundGeneration);
            generationPlaneOffset = config.value("generationPlaneOffset", generationPlaneOffset);
            destructionPlaneOffset = config.value("destructionPlaneOffset", destructionPlaneOffset);
            groundDestructionOffset = config.value("groundDestructionOffset", groundDestructionOffset);
            horizontalDistance = config.value("horizontalDistance", horizontalDistance);
            generationChance = config.value("generationChance", generationChance);
            generationStep = config.value("generationStep", generationStep);
            coinChance = config.value("coinChance", coinChance);
            powerupChance = config.value("powerupChance", powerupChance);
            obstacleChance = config.value("obstacleChance", obstacleChance);
            heartChance = config.value("heartChance", heartChance);
            coinConfig = config.value("coin", nlohmann::json());
            obstacleConfig = config.value("obstacle", nlohmann::json());
            heartConfig = config.value("heart", nlohmann::json());
            powerupConfig = config.value("powerup", nlohmann::json());
            groundConfig = config.value("ground", nlohmann::json());
            groundLength = config.value("groundLength", groundLength);
        }

        // This should be called every frame to update all entities containing a CoinComponent.
        void update(World *world, float deltaTime)
        {
            glm::vec3 playerPosition = glm::vec3(0, 0, 0);
            currentCoinRotation = glm::vec3(0, 0, 0);
            // For each entity in the world get the player position
            for (auto entity : world->getEntities())
            {
                PlayerComponent *player = entity->getComponent<PlayerComponent>();
                if (player)
                    playerPosition = player->getOwner()->localTransform.position;

                CoinTagComponent *coin = entity->getComponent<CoinTagComponent>();
                if (coin)
                    currentCoinRotation = coin->getOwner()->localTransform.rotation;
            }

            // If the number of coins is less than the max number of coins, add a new coin
            while (lastGeneration > playerPosition.z - generationPlaneOffset)
            {
                if (lastGroundGeneration > playerPosition.z - groundLength)
                {
                    Entity *generatedGroundEntity = world->deserializeEntity(groundConfig);
                    generatedGroundEntity->localTransform.position = glm::vec3(0, generatedGroundEntity->localTransform.position.y, lastGroundGeneration);
                    generatedGroundEntity->addComponent<GeneratedComponent>()->destructionOffset = groundDestructionOffset;
                    lastGroundGeneration -= groundLength;
                }
                for (int i = -horizontalDistance / 2; i <= horizontalDistance / 2; ++i)
                {
                    if (distribution(rng) <= generationChance)
                    {
                        Entity *generatedEntity = randomEntityFactory(world, generatedEntity);
                        generatedEntity->localTransform.position = glm::vec3(i, generatedEntity->localTransform.position.y, lastGeneration);
                        generatedEntity->addComponent<GeneratedComponent>()->destructionOffset = destructionPlaneOffset;
                    }
                }
                lastGeneration -= generationStep;
            }

            // For each generated entity in the world
            for (auto entity : world->getEntities())
            {
                GeneratedComponent *generated = entity->getComponent<GeneratedComponent>();
                if (generated)
                {
                    // If the entity is behind the player, mark it for removal
                    if (entity->localTransform.position.z > playerPosition.z - generated->destructionOffset)
                    {
                        world->markForRemoval(entity);
                    }
                }
            }
        }
    };
}
