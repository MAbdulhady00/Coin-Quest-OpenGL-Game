#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"
#include "../components/coin.hpp"
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
    // the generator system is responsible for generating entities in the world
    // this system allows for the generationg and destroying of various entities
    class GeneratorSystem
    {
    private:
        double lastGeneration = 0.0;             // The last generation attempt position
        int generationPlaneOffset = 100;         // The distance from the player at which generated entitites are generated
        int destructionPlaneOffset = -10;        // The distance from the player at which generated entities are destroyed
        int horizontalDistance = 7;              // The horizontal distance from the player at which generated entities are generated
        double generationChance = 0.2;           // The chance that an entity will be generated
        double coinChance = 0.0;                 // The chance that a coin will be generated
        double ancientCoinChance = 0.0;          // The chance that a red coin will be generated
        double blueCoinChance = 0.0;             // The chance that a blue coin will be generated
        double obstacleChance = 0.0;             // The chance that an obstacle will be generated
        double heartChance = 0.0;                // The chance that a heart will be generated
        double powerupChance = 0.0;              // The chance that a powerup will be generated
        double generationStep = 1.0;             // The distance between each generation attempt
        double groundLength = 100.0;             // The length of the ground
        double lastGroundGeneration = 0.0;       // The last ground generated
        double groundDestructionOffset = -100.0; // The distance from the player at which ground is destroyed

        std::mt19937 rng;
        std::uniform_real_distribution<double> distribution;

        nlohmann::json coinConfig;     // The config for the coin
        nlohmann::json obstacleConfig; // The config for the obstacle
        nlohmann::json heartConfig;    // The config for the heart
        nlohmann::json powerupConfig;  // The config for the powerup
        nlohmann::json groundConfig;   // The config for the ground

        glm::vec3 currentCoinRotation; // The current rotation of the coin

        //  Generates a random entity based on rng and the current chances
        inline Entity *randomEntityFactory(World *world)
        {
            // Generate a random number between 0 and 1
            double random = distribution(rng);

            double currentChance = coinChance;
            // If the random number is less than the coin chance, generate a coin
            if (random < currentChance)
            {
                Entity *coin = world->deserializeEntity(coinConfig.value("regular", nlohmann::json()));
                coin->localTransform.rotation = currentCoinRotation;
                return coin;
            }
            currentChance += ancientCoinChance;
            if (random < currentChance)
            {
                Entity *coin = world->deserializeEntity(coinConfig.value("ancient", nlohmann::json()));
                coin->localTransform.rotation = currentCoinRotation;
                return coin;
            }
            currentChance += blueCoinChance;
            if (random < currentChance)
            {
                Entity *coin = world->deserializeEntity(coinConfig.value("blue", nlohmann::json()));
                coin->localTransform.rotation = currentCoinRotation;
                return coin;
            }
            currentChance += obstacleChance;
            // If the random number is less than the obstacle chance, generate an obstacle
            if (random < currentChance)
            {
                return world->deserializeEntity(obstacleConfig);
            }
            currentChance += powerupChance;
            // If the random number is less than the powerup chance, generate a powerup
            if (random < currentChance)
            {
                return world->deserializeEntity(powerupConfig);
            }
            // must generate a heart
            return world->deserializeEntity(heartConfig);
        }

    public:
        GeneratorSystem()
        {
            std::random_device rand_dev;
            rng = std::mt19937(rand_dev());
            // initialize the distribution to be between 0 and 1
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
            ancientCoinChance = config.value("ancientCoinChance", ancientCoinChance);
            blueCoinChance = config.value("blueCoinChance", blueCoinChance);
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
                    playerPosition = player->getOwner()->getWorldTranslation();

                CoinComponent *coin = entity->getComponent<CoinComponent>();
                if (coin)
                    currentCoinRotation = coin->getOwner()->localTransform.rotation;
            }

            // If the number of coins is less than the max number of coins, add a new coin
            while (lastGeneration > playerPosition.z - generationPlaneOffset)
            {
                // generate ground if the last ground is too far away
                if (lastGroundGeneration > playerPosition.z - groundLength)
                {
                    Entity *generatedGroundEntity = world->deserializeEntity(groundConfig);
                    // set the position of the ground
                    generatedGroundEntity->localTransform.position = glm::vec3(0, generatedGroundEntity->localTransform.position.y, lastGroundGeneration);
                    generatedGroundEntity->addComponent<GeneratedComponent>()->destructionOffset = groundDestructionOffset;
                    // Move the ground generation plane forward
                    lastGroundGeneration -= groundLength;
                }

                // generate entities in a grid pattern
                for (int i = -horizontalDistance / 2; i <= horizontalDistance / 2; ++i)
                {
                    // If the random number is less than the generation chance, generate an entity
                    if (distribution(rng) <= generationChance)
                    {
                        Entity *generatedEntity = randomEntityFactory(world);
                        // set the position of the entity
                        generatedEntity->localTransform.position = glm::vec3(i, generatedEntity->localTransform.position.y, lastGeneration);
                        generatedEntity->addComponent<GeneratedComponent>()->destructionOffset = destructionPlaneOffset;
                    }
                }

                // Move the generation plane forward
                lastGeneration -= generationStep;
            }

            // For each generated entity in the world
            for (auto entity : world->getEntities())
            {
                GeneratedComponent *generated = entity->getComponent<GeneratedComponent>();
                if (generated)
                {
                    // If the entity is behind the player, mark it for removal
                    if (entity->getWorldTranslation().z > playerPosition.z - generated->destructionOffset)
                    {
                        world->markForRemoval(entity);
                    }
                }
            }
        }
    };
}
